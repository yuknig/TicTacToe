#pragma once
#include "IPlayerController.h"
#include "GameTypes.h"
#include <memory>
#include <optional>

class Board;

class HumanPlayerController : public IPlayerController
{
public:
    bool MoveReady() override;
    Point GetMove() override;

    void OnCellPressed(const Point& p); // Forwarded from BoardWidget through GameControl

private:
    std::optional<Point> m_movePoint;
};

class ComputerPlayerController : public IPlayerController
{
public:
    ComputerPlayerController(std::weak_ptr<Board> board, GameType gameType, PlayerRole player);

    bool MoveReady() override;
    Point GetMove() override;

private:
    const std::weak_ptr<Board> m_board;
    const GameType m_gameType;
    const PlayerRole m_player;
};
