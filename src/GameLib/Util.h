#pragma once
#include "GameTypes.h"
#include "GeomTypes.h"
#include <utility>

inline uint8_t GetLineLengthToWin(GameType type)
{
    return type == GameType::ThreeInRow ? 3 : 5;
}

inline BoardCellState GetCellTypeForPlayer(PlayerRole player)
{
    return player == PlayerRole::X ? BoardCellState::X : BoardCellState::O;
}

inline PlayerRole GetRoleFromPlayerNum(uint8_t playerNum)
{
    return playerNum == 1 ? PlayerRole::O : PlayerRole::X;
}


enum LineDirection
{
    Right,
    RightDown,
    Down,
    LeftDown,
};

constexpr inline std::pair<int, int> DirectionPosIncrement(LineDirection dir)
{
    switch (dir)
    {
    case LineDirection::Right:
        return { 1, 0 };
    case LineDirection::RightDown:
        return { 1, 1 };
    case LineDirection::Down:
        return { 0, 1 };
    case LineDirection::LeftDown:
        return { -1, 1 };
    }

    return { 0, 0 };
}
