#include "GameControl.h"
#include "GameModel.h"
#include "IGameView.h"
#include "IStatusMessage.h"
#include "PlayerControllers.h"
#include "Util.h"

namespace {

std::string GetStatusString(const GameModel& model)
{
    switch (auto state = model.GetState())
    {
    case GameState::MoveX:
        return "X to move";
    case GameState::MoveO:
        return "O to move";
    case GameState::WonX:
        return "X won";
    case GameState::WonO:
        return "O won";
    case GameState::Draw:
        return "Draw";
    }

    return "";
}

} // namespace anonymous


GameControl::GameControl(IGameView* view, IStatusMessage* statusMsg)
    : m_view(view)
    , m_statusMsg(statusMsg)
{}

void GameControl::StartNewGame(const NewGameSettings& settings)
{
    // Store settings
    m_settings = settings;

    // Recreate model for a new game
    m_model = m_settings.m_gameType == GameType::ThreeInRow
                ? GameModel::Create3InRow()
                : GameModel::Create5InRow(m_settings.m_boardSize[0], m_settings.m_boardSize[1]);
    m_view->SetGameModel(m_model);
    m_model->SetView(m_view);

    // Create player controllers
    for (uint8_t playerNum = 0; playerNum < 2; ++playerNum)
    {
        PlayerType playerType = m_settings.m_players[playerNum];
        if (playerType == PlayerType::Human)
            m_playerControllers[playerNum] = std::make_unique<HumanPlayerController>();
        else
        {
            auto player = GetRoleFromPlayerNum(playerNum);
            m_playerControllers[playerNum] = std::make_unique<ComputerPlayerController>(m_model->GetBoard(), m_settings.m_gameType, player);
        }
    }

    // Start game
    m_model->StartGame();
    UpdateStatusMsg();
}

void GameControl::OnBoardCellPressed(const Point& p)
{
    // Handle click on board done by human player
    if (!m_model)
        return;

    const std::optional playerId = m_model->GetCurrentMovePlayerId();
    if (!playerId)
        return;

    auto& controller = m_playerControllers[*playerId];
    if (auto humanController = dynamic_cast<HumanPlayerController*>(controller.get()))
    {
        humanController->OnCellPressed(p);
        Update();
    }
}

void GameControl::Update()
{
    if (std::optional playerId = m_model->GetCurrentMovePlayerId())
    {
        auto& controller = m_playerControllers.at(*playerId);
        if (controller->MoveReady())
            MakeMove(controller->GetMove());
    }
}

void GameControl::MakeMove(const Point& p)
{
    m_model->MakeMove(p);
    UpdateStatusMsg();
}

void GameControl::UpdateStatusMsg()
{
    if (!m_statusMsg || !m_model)
        return;

    m_statusMsg->ShowStatusMessage(GetStatusString(*m_model));
}
