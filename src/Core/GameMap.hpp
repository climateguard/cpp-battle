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
        
        // Position -> список UnitId
        std::unordered_map<uint64_t, std::vector<UnitId>> _positionToUnits;

    public:
        GameMap() = default;
        GameMap(uint32_t width, uint32_t height)
            : _width(width), _height(height) {}

        uint32_t getWidth() const { return _width; }
        uint32_t getHeight() const { return _height; }

        bool isValidPosition(const Position& pos) const
        {
            return pos.x < _width && pos.y < _height;
        }

        // place unit on game map
        void placeUnit(const Position& pos, UnitId id)
        {
            _positionToUnits[positionToKey(pos)].push_back(id);
        }

        void removeUnit(const Position& pos, UnitId id)
        {
            auto key = positionToKey(pos);
            auto it = _positionToUnits.find(key);
            if (it == _positionToUnits.end())
                return;

            auto& vec = it->second;
            vec.erase(std::remove(vec.begin(), vec.end(), id), vec.end());
            
            if (vec.empty())
                _positionToUnits.erase(it);
        }

        void moveUnit(const Position& from, const Position& to, UnitId id)
        {
            removeUnit(from, id);
            placeUnit(to, id);
        }

        UnitId getUnitAt(const Position& pos, UnitId id) const
        {
            auto it = _positionToUnits.find(positionToKey(pos));
            if (it != _positionToUnits.end() && !it->second.empty())
                return it->second[0];
            return UnitId{0};
        }

        std::vector<UnitId> getAllUnitsAt(const Position& pos) const
        {
            auto it = _positionToUnits.find(positionToKey(pos));
            return (it != _positionToUnits.end()) ? it->second : std::vector<UnitId>{};
        }

    private:
        // Convert x/y position to hash table key
        uint64_t positionToKey(const Position& pos) const
        {
            return (static_cast<uint64_t>(pos.x) << 32) | pos.y;
        }
    };
}