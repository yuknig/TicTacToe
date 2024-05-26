#include "GameModel.h"
#include "IGameView.h"
#include "Board.h"
#include <stdexcept>

std::unique_ptr<GameModel> GameModel::Create3InRow()
{
    return std::unique_ptr<GameModel>(new GameModel(GameType::ThreeInRow, 3, 3));
}

std::unique_ptr<GameModel> GameModel::Create5InRow(uint8_t boardWidth, uint8_t boardHeight)
{
    return std::unique_ptr<GameModel>(new GameModel(GameType::FiveInRow, boardWidth, boardHeight));
}


GameModel::GameModel(GameType gameType, uint8_t boardWidth, uint8_t boardHeight)
    : m_gameType(gameType)
    , m_board(std::make_shared<Board>(boardWidth, boardHeight))
{}

void GameModel::SetView(IGameView* view)
{
    m_view = view;
    if (m_view)
        m_view->UpdateView();
}

std::shared_ptr<Board> GameModel::GetBoard() const
{
    return m_board;
}

void GameModel::StartGame()
{
    if (m_state != GameState::None)
        throw std::logic_error("Model is in wrong state");

    m_state = GameState::MoveX;
}

void GameModel::MakeMove(const Point& pos)
{
    if (!m_board || !m_board->IsCellBlank(pos))
        return; // The cell is already occupied

    if (!IsMoveState())
        return; // Not in state of X/O move

    const BoardCellState newCellState = (m_state == GameState::MoveX) ? BoardCellState::X : BoardCellState::O;
    m_board->SetCell(pos, newCellState);

    if (m_board->PlayerWon(m_gameType, newCellState))
        m_state = (m_state == GameState::MoveX) ? GameState::WonX : GameState::WonO;
    else if (m_board->IsFilled( ))
        m_state = GameState::Draw;
    else
        m_state = (m_state == GameState::MoveX) ? GameState::MoveO : GameState::MoveX;

    if (m_view)
        m_view->UpdateView();
}

GameType GameModel::GetGameType() const
{
    return m_gameType;
}

GameState GameModel::GetState() const
{
    return m_state;
}

bool GameModel::IsMoveState() const
{
    return m_state == GameState::MoveX ||
           m_state == GameState::MoveO;
}

bool GameModel::IsWonState() const
{
    return m_state == GameState::WonX ||
           m_state == GameState::WonO;
}

std::optional<uint8_t> GameModel::GetCurrentMovePlayerId() const
{
    switch (m_state)
    {
    case GameState::MoveX:
        return 0;
    case GameState::MoveO:
        return 1;
    }

    return std::nullopt;
}
