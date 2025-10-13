#pragma once

#include <cstdint>
#include <cmath>

namespace sw::core
{
    struct Position
    {
        uint32_t x{};
        uint32_t y{};

        bool operator==(const Position& other) const
        {
            return x == other.x && y == other.y;
        }

        bool operator!=(const Position& other) const
        {
            return !(*this == other);
        }

        // minimum number of steps to target position
        uint32_t distanceTo(const Position& other) const
        {
            uint32_t dx = std::abs(static_cast<int32_t>(x) - static_cast<int32_t>(other.x));
            uint32_t dy = std::abs(static_cast<int32_t>(y) - static_cast<int32_t>(other.y));
            return std::max(dx, dy);
        }

        Position stepTowards(const Position& target) const {
            Position next = *this;

            if (x < target.x) next.x += 1;
            else if (x > target.x) next.x -= 1;

            if (y < target.y) next.y += 1;
            else if (y > target.y) next.y -= 1;

            return next;
        }
    };
} 