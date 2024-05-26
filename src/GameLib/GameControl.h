#pragma once
#include "GameTypes.h"
#include "IPlayerController.h"
#include <array>
#include <memory>

class GameModel;
class IGameView;
class IStatusMessage;

class GameControl
{
public:
    GameControl(IGameView* view, IStatusMessage* statusMsg);

    void StartNewGame(const NewGameSettings& settings);

    void OnBoardCellPressed(const Point& p);
    void Update();

private:
    void MakeMove(const Point& p);
    void UpdateStatusMsg();

private:
    IGameView* const m_view;
    IStatusMessage* const m_statusMsg;

    NewGameSettings m_settings = {};
    std::shared_ptr<GameModel> m_model;
    std::array<std::unique_ptr<IPlayerController>, 2> m_playerControllers;
};
