#pragma once
#include "GameTypes.h"
#include "GeomTypes.h"
#include <memory>
#include <optional>

class Board;

class ComputerPlayerMove
{
public:
    ComputerPlayerMove(std::shared_ptr<Board> board, BoardCellState player, uint8_t lineLengthToWin, uint8_t maxDepth = 8);

    [[nodiscard]] Point GetMove();

private:
    std::optional<Point> FindGoodMove();
    int CalcMoveScore(const Point& movePos, BoardCellState thisMovePlayer, Rect curBRect, int curDepth);
    std::optional<int> GetMoveWinScore(const Point& movePos, BoardCellState thisMovePlayer, int curDepth) const;

private:
    const std::shared_ptr<Board> m_board;
    const BoardCellState m_player;
    const uint8_t m_lineLengthToWin;
    const uint8_t m_maxDepth = 8;
};
