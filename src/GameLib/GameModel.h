#pragma once
#include "GameTypes.h"
#include "GeomTypes.h"
#include <memory>
#include <optional>

class Board;
class IGameView;

class GameModel
{
public:
    static std::unique_ptr<GameModel> Create3InRow();
    static std::unique_ptr<GameModel> Create5InRow(uint8_t boardWidth, uint8_t boardHeight);

    void SetView(IGameView* view);

    std::shared_ptr<Board> GetBoard() const;

    void StartGame();
    void MakeMove(const Point& pos);

    GameType GetGameType() const;

    GameState GetState() const;
    bool IsMoveState() const;
    std::optional<uint8_t> GetCurrentMovePlayerId() const;

private:
    GameModel(GameType gameType, uint8_t boardWidth, uint8_t boardHeight);

    const GameType m_gameType;
    GameState m_state = GameState::None;
    const std::shared_ptr<Board> m_board;
    IGameView* m_view = nullptr;
};
