#pragma once
#include "GameTypes.h"
#include "GeomTypes.h"
#include <vector>
#include <optional>

class Board
{
public:
    Board(uint8_t width, uint8_t height);

    uint8_t Columns() const;
    uint8_t Rows() const;

    void SetCell(const Point& p, BoardCellState state);
    void ClearCell(const Point& p);
    BoardCellState GetCell(const Point& cellPos) const;

    bool IsCellBlank(const Point& p) const;
    bool CellHasOccupiedNeighbour(const Point& p) const;

    bool IsBlank() const;
    bool IsFilled() const;
    bool PlayerWon(GameType type, BoardCellState playerCellState) const;

    std::optional<Line> FindLine(const BoardCellState state, uint8_t minLength) const;
    std::optional<Rect> GetBoundingRect() const;

private:
    BoardCellState& GetCellRef(const Point& p);
    const BoardCellState& GetCellRef(const Point& p) const;
    uint16_t GetCellOffset(const Point& p) const;

private:
    const uint8_t m_width;
    const uint8_t m_height;
    std::vector<BoardCellState> m_cells;
};
