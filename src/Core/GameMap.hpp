#pragma once

#include "Position.hpp"
#include "UnitId.hpp"
#include <unordered_map>
#include <cstdint>

namespace sw::core
{
    // Game map
    class GameMap
    {
    private:
        uint32_t _width{};
        uint32_t _height{};
        
        // Position to UnitId (for seach unit in map)
        std::unordered_map<uint64_t, UnitId> _positionToUnit;

    public:
        GameMap() = default;
        GameMap(uint32_t width, uint32_t height)
            : _width(width), _height(height)
        {}

        uint32_t getWidth() const { return _width; }
        uint32_t getHeight() const { return _height; }

        bool isValidPosition(const Position& pos) const
        {
            return pos.x < _width && pos.y < _height;
        }

        // place unit on game map
        void placeUnit(const Position& pos, UnitId id)
        {
            _positionToUnit[positionToKey(pos)] = id;
        }

        // remove unit from game map
        void removeUnit(const Position& pos)
        {
            _positionToUnit.erase(positionToKey(pos));
        }

        // Move unit on game map
        void moveUnit(const Position& from, const Position& to, UnitId id)
        {
            removeUnit(from);
            placeUnit(to, id);
        }

        // Get unit on position
        UnitId getUnitAt(const Position& pos) const
        {
            auto it = _positionToUnit.find(positionToKey(pos));
            return (it != _positionToUnit.end()) ? it->second : UnitId{0};
        }

    private:
        // Convert x/y position to hash table key
        uint64_t positionToKey(const Position& pos) const
        {
            return (static_cast<uint64_t>(pos.x) << 32) | pos.y;
        }
    };
}