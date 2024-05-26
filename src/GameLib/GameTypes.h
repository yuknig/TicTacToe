#pragma once
#include <array>
#include <cstdint>

enum class GameType
{
    ThreeInRow,
    FiveInRow,
};

enum class GameState
{
    None,
    MoveX,
    MoveO,
    WonX,
    WonO,
    Draw,
};


enum class BoardCellState
{
    Blank,
    X,
    O,
};

enum class PlayerRole
{
    X,
    O,
};

enum class PlayerType
{
    Human,
    Computer,
};

struct NewGameSettings
{
    GameType m_gameType = GameType::ThreeInRow;
    std::array<uint8_t, 2> m_boardSize = { 0, 0}; // applies to FiveInRow game only
    std::array<PlayerType, 2> m_players = { PlayerType::Human, PlayerType::Computer };
};
