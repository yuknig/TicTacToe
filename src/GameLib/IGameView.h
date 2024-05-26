#pragma once
#include <memory>

class GameModel;
class GameControl;

class IGameView
{
public:
    virtual ~IGameView() = default;

    virtual void SetGameModel(std::weak_ptr<GameModel> model) = 0;
    virtual void SetGameControl(std::weak_ptr<GameControl> control) = 0;

    virtual void UpdateView() = 0;
};
