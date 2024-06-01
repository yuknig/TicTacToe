#include "ComputerPlayerMove.h"
#include "Board.h"
#include "Util.h"
#include <cassert>
#include <stdexcept>

namespace
{

struct MoveCanidate
{
    Point p;
    int score;
};

Rect DecrementedMargin(Rect rect, uint8_t maxX, uint8_t maxY)
{
    if (rect.lt.x > 0)
        --rect.lt.x;
    if (rect.lt.y > 0)
        --rect.lt.y;

    if (rect.rb.x < maxX)
        ++rect.rb.x;
    if (rect.rb.y < maxY)
        ++rect.rb.y;

    return rect;
}

} // namespace anonymous


ComputerPlayerMove::ComputerPlayerMove(std::shared_ptr<Board> board, BoardCellState player, uint8_t lineLengthToWin, uint8_t maxDepth)
    : m_board(std::move(board))
    , m_player(player)
    , m_lineLengthToWin(lineLengthToWin)
    , m_maxDepth(maxDepth)
{}

Point ComputerPlayerMove::GetMove()
{
    if (!m_board)
        throw std::runtime_error("Board model not provided");

    if (auto move = FindGoodMove())
        return *move;

    assert(m_board->IsBlank());
    return { static_cast<uint8_t>(m_board->Columns() / 2),
             static_cast<uint8_t>(m_board->Rows() / 2) }; // place in the middle of board
}

std::optional<Point> ComputerPlayerMove::FindGoodMove()
{
    auto& board = *m_board;

    std::vector<MoveCanidate> bestCandidates;

    // Narrow region of interest to speedup work on big boards
    Rect brect;
    if (auto optBrect = board.GetBoundingRect())
        brect = optBrect.value();
     else
        return std::nullopt;

    // Extend bounding rect by 1 to iterate blank neighbors of occupied cells
    const Rect checkRect = DecrementedMargin(brect, m_board->Columns() - 1u, m_board->Rows() - 1u);
    for (uint8_t y = checkRect.Top(); y <= checkRect.Bottom(); ++y)
    {
        for (uint8_t x = checkRect.Left(); x <= checkRect.Right(); ++x)
        {
            const Point p{ x, y };

            if (board.IsCellBlank(p) && board.CellHasOccupiedNeighbour(p))
            {
                const int score = CalcMoveScore(p, m_player, brect, 0);
                if (bestCandidates.empty())
                    bestCandidates.push_back({ p, score });
                else if (score > bestCandidates.front().score)
                {
                    bestCandidates.clear();
                    bestCandidates.push_back({ p, score });
                }
            }
        }
    }

    if (bestCandidates.empty())
        throw std::logic_error("None of move candidates found");

    // Select randomly among candidates with the best score
    return bestCandidates[rand() % bestCandidates.size()].p;
}

int ComputerPlayerMove::CalcMoveScore(const Point& movePos, BoardCellState thisMovePlayer, Rect curBRect, int curDepth)
{
    // Implementation is based on Minimax algorithm

    // Check if this move wins the game
    if (auto thisMoveScore = GetMoveWinScore(movePos, thisMovePlayer, curDepth))
        return *thisMoveScore;

    if (curDepth >= m_maxDepth)
        return 0; // Max Depth reached

    // This move does not win the game.
    // So do the next move for opponent

    auto& board = *m_board;
    board.SetCell(movePos, thisMovePlayer); // Make move
    curBRect.IncludePoint(movePos);

    const BoardCellState opponent = thisMovePlayer == BoardCellState::X ? BoardCellState::O : BoardCellState::X;

    const bool findMax = m_player == opponent;
    const int initScore = findMax ? INT_MIN : INT_MAX;
    int bestScore = initScore;

    // Extend bounding rect by 1 to iterate blank neighbors of occupied cells
    const Rect checkRect = DecrementedMargin(curBRect, m_board->Columns() - 1u, m_board->Rows() - 1u);
    for (uint8_t y = checkRect.Top(); y <= checkRect.Bottom(); ++y)
    {
        for (uint8_t x = checkRect.Left(); x <= checkRect.Right(); ++x)
        {
            const Point p{ x, y };

            if (board.IsCellBlank(p) && board.CellHasOccupiedNeighbour(p))
            {
                const auto score = CalcMoveScore(p, opponent, curBRect, curDepth + 1);
                if (findMax)
                    bestScore = std::max(bestScore, score);
                else
                    bestScore = std::min(bestScore, score);
            }
        }
    }

    board.ClearCell(movePos); // Undo move

    return (bestScore == initScore) ? 0 : bestScore;
}

std::optional<int> ComputerPlayerMove::GetMoveWinScore(const Point& movePos, BoardCellState thisMovePlayer, int curDepth) const
{
    const int WinningScore = m_maxDepth + 1;

    auto& board = *m_board;

    const uint8_t width = board.Columns();
    const uint8_t height = board.Rows();

    for (auto lineDir : { LineDirection::Right, LineDirection::RightDown, LineDirection::Down, LineDirection::LeftDown }) {
        const auto [incrX, incrY] = DirectionPosIncrement(lineDir);

        int len = 1;

        // First go in checked line direction
        for (int offset = 1; offset < m_lineLengthToWin; ++offset)
        {
            const int curX = movePos.x + incrX*offset;
            const int curY = movePos.y + incrY*offset;
            if (curX < 0 || curX >= width ||
                curY < 0 || curY >= height ||
                board.GetCell({ static_cast<uint8_t>(curX), static_cast<uint8_t>(curY) }) != thisMovePlayer)
                break;
            ++len;
        }

        // Then go in the opposite direction
        for (int offset = 1; offset < m_lineLengthToWin; ++offset)
        {
            const int curX = movePos.x - incrX*offset;
            const int curY = movePos.y - incrY*offset;
            if (curX < 0 || curX >= width ||
                curY < 0 || curY >= height ||
                board.GetCell({ static_cast<uint8_t>(curX), static_cast<uint8_t>(curY) }) != thisMovePlayer)
                break;
            ++len;
        }

        if (len >= m_lineLengthToWin)
        {
            // The move actually wins the game for this player
            int score = WinningScore - curDepth;
            return m_player == thisMovePlayer ? score : -score;
        }
    }

    // This move does not win the game.
    return std::nullopt;
}
