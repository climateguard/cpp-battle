#pragma once

#include "Position.hpp"
#include "UnitId.hpp"
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <cstdint>

namespace sw::core
{
    class IUnit;
    enum class Unitproperty;

    // Game map
    class GameMap
    {
    private:
        uint32_t _width{};
        uint32_t _height{};
        
        // Position -> UnitId
        std::unordered_map<uint64_t, std::vector<UnitId>> _positionToUnits;
        
        std::unordered_map<UnitId, IUnit*> _unitById;

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

        void registerUnit(UnitId id, IUnit* unit)
        {
            _unitById[id] = unit;
        }

        void unregisterUnit(UnitId id)
        {
            _unitById.erase(id);
        }

        bool canPlaceUnit(const Position& pos, UnitId id)
        {
            auto* unit = findUnitById(id);
            if (!unit)
                return false;

            if (!unit->has_property(Unitproperty::NotOccupy))
                return true;

            auto ids = getAllUnitsAt(pos);
            for (auto existingId : ids)
            {
                auto* existingUnit = findUnitById(existingId);
                if (existingUnit && !existingUnit->has_property(Unitproperty::NotOccupy))
                    return false;
            }

            return true;
        }

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

        IUnit* findUnitAt(const Position& pos)
        {
            auto ids = getAllUnitsAt(pos);
            for(auto id : ids){
                if(id.isValid() && !_unitById[id]->has_property(Unitproperty::Transparent)){
                    return _unitById[id];
                }
            }
            return nullptr;
        }

        std::vector<IUnit*> findUnitsAt(const Position& pos)
        {
            std::vector<IUnit*> res;
            auto ids = getAllUnitsAt(pos);
            for(auto id : ids){
                if(id.isValid() && !_unitById[id]->has_property(Unitproperty::Transparent)){
                    res.push_back(_unitById[id]);
                }
            }
            return res;
        }

        //return all units, include Transparent
        IUnit* findUnitById(UnitId id)
        {
            auto it = _unitById.find(id);
            return (it != _unitById.end()) ? it->second : nullptr;
        }

        std::vector<IUnit*> findUnitsInRange(const Position& center, uint32_t range)
        {
            std::vector<IUnit*> result;
            
            for (auto& [id, unit] : _unitById)
            {
                if (!unit->isDead() && !unit->has_property(Unitproperty::Transparent))
                {
                    Position unitPos = unit->getPosition();
                    if (unitPos.distanceTo(center) <= range && unitPos != center)
                    {
                        result.push_back(unit);
                    }
                }
            }
            
            return result;
        }

        std::vector<IUnit*> findAdjacentUnits(const Position& pos)
        {
            return findUnitsInRange(pos, 1);
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