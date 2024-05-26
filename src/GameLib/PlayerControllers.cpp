#include "PlayerControllers.h"
#include "Board.h"
#include "ComputerPlayerMove.h"
#include "Util.h"
#include <cassert>

bool HumanPlayerController::MoveReady()
{
    return m_movePoint.has_value();
}

Point HumanPlayerController::GetMove()
{
    assert(m_movePoint.has_value());

    Point p = *m_movePoint;
    m_movePoint.reset();
    return p;
}

void HumanPlayerController::OnCellPressed(const Point& p)
{
    m_movePoint = p;
}


ComputerPlayerController::ComputerPlayerController(std::weak_ptr<Board> board, GameType gameType, PlayerRole player)
    : m_board(std::move(board))
    , m_gameType(gameType)
    , m_player(player)
{}

bool ComputerPlayerController::MoveReady()
{
    return true; // Result is ready when requested at any time
}

Point ComputerPlayerController::GetMove()
{
    // Make a copy of board to make sure that we don't spoil the shared one
    const auto board = std::make_shared<Board>(*m_board.lock());
    const BoardCellState player = GetCellTypeForPlayer(m_player);
    const uint8_t lineLength = GetLineLengthToWin(m_gameType);

    // TODO: implement better AI for FiveInRow
    const uint8_t MaxDepth = m_gameType == GameType::ThreeInRow ? 8 : 4;

    ComputerPlayerMove playerMove(board, player, lineLength, MaxDepth);
    return playerMove.GetMove();
}
