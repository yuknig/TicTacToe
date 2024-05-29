#include "Board.h"
#include "Util.h"
#include <algorithm>
#include <cassert>
#include <stdexcept>

Board::Board(uint8_t width, uint8_t height)
    : m_width(width)
    , m_height(height)
    , m_cells(width* height, BoardCellState::Blank)
{}

uint8_t Board::Columns() const
{
    return m_width;
}

uint8_t Board::Rows() const
{
    return m_height;
}

void Board::SetCell(const Point& p, BoardCellState state)
{
    auto& cell = GetCellRef(p);
    if (cell != BoardCellState::Blank)
        throw std::runtime_error("Cell was already set");

    cell = state;
}

void Board::ClearCell(const Point& p)
{
    auto& cell = GetCellRef(p);
    cell = BoardCellState::Blank;
}

BoardCellState Board::GetCell(const Point& p) const
{
    return GetCellRef(p);
}

BoardCellState& Board::GetCellRef(const Point& p)
{
    if (p.x >= m_width || p.y >= m_height)
        throw std::runtime_error("Cell coordinates are out of range");

    return m_cells[GetCellOffset(p)];
}

const BoardCellState& Board::GetCellRef(const Point& p) const
{
    if (p.x >= m_width || p.y >= m_height)
        throw std::runtime_error("Cell coordinates are out of range");

    return m_cells[GetCellOffset(p)];
}

uint16_t Board::GetCellOffset(const Point& p) const
{
    const uint16_t offset = p.y * m_width + p.x;
    assert(offset < m_cells.size());
    return offset;
}

bool Board::IsCellBlank(const Point& p) const
{
    return GetCell(p) == BoardCellState::Blank;
}

bool Board::CellHasOccupiedNeighbour(const Point& p) const
{
    const auto [x, y] = p;
    const auto idx = GetCellOffset(p);

    const bool hasPixelOnLeft = x > 0;
    if (hasPixelOnLeft && m_cells[idx - 1] != BoardCellState::Blank)
        return true;

    const bool hasPixelOnRight = x + 1 < m_width;
    if (hasPixelOnRight && m_cells[idx + 1] != BoardCellState::Blank)
        return true;

    const bool hasPixelAbove = y > 0;
    if (hasPixelAbove)
    {
        if (hasPixelOnLeft && m_cells[idx - m_width -  1] != BoardCellState::Blank)
            return true;
        if (m_cells[idx - m_width] != BoardCellState::Blank)
            return true;
        if (hasPixelOnRight && m_cells[idx - m_width + 1] != BoardCellState::Blank)
            return true;
    }

    const bool hasPixelBelow = y + 1 < m_height;
    if (hasPixelBelow)
    {
        if (hasPixelOnLeft && m_cells[idx + m_width - 1] != BoardCellState::Blank)
            return true;
        if (m_cells[idx + m_width] != BoardCellState::Blank)
            return true;
        if (hasPixelOnRight && m_cells[idx + m_width + 1] != BoardCellState::Blank)
            return true;
    }

    return false;
}

bool Board::IsBlank() const
{
    return std::all_of(m_cells.begin(), m_cells.end(),
        [](const BoardCellState state) {
            return state == BoardCellState::Blank;
    });
}

bool Board::IsFilled() const
{
    return std::none_of(m_cells.begin(), m_cells.end(),
        [](const BoardCellState state) {
            return state == BoardCellState::Blank;
        });
}

bool Board::PlayerWon(GameType type, BoardCellState playerCellState) const
{
    const uint8_t lengthToWin = GetLineLengthToWin(type);
    std::optional line = FindLine(playerCellState, lengthToWin);
    return line.has_value();
}

std::optional<Line> Board::FindLine(const BoardCellState state, uint8_t minLength) const
{
    if (!m_width || !m_height || (m_width < minLength && m_height < minLength))
        return std::nullopt;

    uint16_t idx = 0;
    for (uint8_t y = 0; y < m_height; ++y) {
        for (uint8_t x = 0; x < m_width; ++x, ++idx) {
            const auto curState = m_cells[idx];
            if (curState != state)
                continue;

            for (auto lineDir : { LineDirection::Right, LineDirection::RightDown, LineDirection::Down, LineDirection::LeftDown }) {
                const auto [incrX, incrY] = DirectionPosIncrement(lineDir);

                const int prevX = x - incrX;
                const int prevY = y - incrY;
                if (prevX >= 0 && prevX < m_width &&
                    prevY >= 0 && prevY < m_height &&
                    m_cells[prevY*m_width + prevX] == state)
                    continue; // No need to detect line starting from this point because it was already handled earlier

                uint8_t len = minLength;
                for (int offset = 1; offset < minLength; ++offset) {
                    const int curX = x + incrX * offset;
                    const int curY = y + incrY * offset;
                    if (curX < 0 || curY < 0 ||
                        curX >= m_width || curY >= m_height ||
                        m_cells[curY*m_width + curX] != state)
                    {
                        len = offset;
                        break;
                    }
                }

                if (len >= minLength)
                    return Line{ Point{ x, y }, lineDir, len };
            }
        }
    }

    return std::nullopt;
}


std::optional<Rect> Board::GetBoundingRect() const
{
    // Use lambdas to be able to break through 2 nested loops
    const std::optional optTop = [&]() -> std::optional<uint8_t> 
    {
        size_t idx = 0u;
        for (uint8_t y = 0; y < m_height; ++y)
            for (uint8_t x = 0; x < m_width; ++x, ++idx)
                if (m_cells[idx] != BoardCellState::Blank)
                    return y;

        return std::nullopt;
    }();
    if (!optTop)
        return std::nullopt; // return nullopt if board is blank

    const uint8_t top = *optTop;

    const uint8_t bottom = [&]() -> uint8_t
    {
        size_t idx = m_width*m_height - 1;
        for (uint8_t y = 0; y < m_height; ++y)
            for (uint8_t x = 0; x < m_width; ++x, --idx)
                if (m_cells[idx] != BoardCellState::Blank)
                    return m_height - y - 1;
        return 0; // unreachable
    }();

    const uint8_t left = [&]() -> uint8_t
    {
        const size_t startIdx = top * m_width;
        for (uint8_t x = 0; x < m_width; ++x)
        {
            size_t idx = startIdx + x;
            for (uint8_t y = top; y <= bottom; ++y, idx += m_width)
                if (m_cells[idx] != BoardCellState::Blank)
                    return x;
        }
        return 0;
    }();

    const uint8_t right = [&]() -> uint8_t
    {
        const size_t startIdx = top*m_width + m_width - 1;
        for (uint8_t x = 0; x < m_width; ++x)
        {
            size_t idx = startIdx - x;
            for (uint8_t y = top; y <= bottom; ++y, idx += m_width)
                if (m_cells[idx] != BoardCellState::Blank)
                    return m_width - x - 1;
        }
        return 0;
    }();

    return Rect{ Point{left, top}, Point{right, bottom} };
}
