#pragma once
#include <cstdint>
#include "util.h"

struct Point
{
    uint8_t x = 0;
    uint8_t y = 0;
};

struct Line
{
    Point start;
    LineDirection dir;
    uint8_t length;

    const Point& Start() const
    {
        return start;
    }

    Point End() const
    {
        Point end = start;
        const auto [incrX, incrY] = DirectionPosIncrement(dir);
        end.x += incrX * (length - 1);
        end.y += incrY * (length - 1);
        return end;
    }
};


struct Rect
{
    Rect() = default;
    Rect(const Point& p1, const Point& p2)
        : lt(p1)
        , rb(p2)
    {
        if (lt.x > rb.x)
            std::swap(lt.x, rb.x);
        if (lt.y > rb.y)
            std::swap(lt.y, rb.y);
    }

    uint8_t Left() const
    {
        return lt.x;
    }

    uint8_t Right() const
    {
        return rb.x;
    }

    uint8_t Top() const
    {
        return lt.y;
    }

    uint8_t Bottom() const
    {
        return rb.y;
    }

    void IncludePoint(const Point& p)
    {
        if (p.x < lt.x)
            lt.x = p.x;
        else if (p.x > rb.x)
            rb.x = p.x;

        if (p.y < lt.y)
            lt.y = p.y;
        else if (p.y > rb.y)
            rb.y = p.y;
    }

    Point lt;
    Point rb;
};
