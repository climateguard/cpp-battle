#pragma once

#include "IGameContext.hpp"
#include "IUnit.hpp"
#include "GameMap.hpp"
#include "../IO/System/EventLog.hpp"
#include "../IO/Events/MapCreated.hpp"
#include "../IO/Events/UnitSpawned.hpp"
#include "../IO/Events/UnitDied.hpp"
#include "../IO/Events/UnitMoved.hpp"
#include "../IO/Events/UnitAttacked.hpp"
#include "../IO/Events/MarchStarted.hpp"
#include "../IO/Events/MarchEnded.hpp"

#include <memory>
#include <vector>
#include <unordered_map>
#include <algorithm>

namespace sw::core
{
    // Main simulation class
    class GameWorld : public IGameContext
    {
    private:
        GameMap _map;
        EventLog _eventLog;
        uint64_t _currentTick{0};
        // keep units in order of creation
        std::vector<std::unique_ptr<IUnit>> _units;
        
        // connect unit by id
        std::unordered_map<UnitId, IUnit*> _unitById;

    public:

        EventLog& getEventLog() override { return _eventLog; }
        uint64_t get_tick() const override { return _currentTick; }

        void createMap(uint32_t width, uint32_t height)
        {
            _map = GameMap(width, height);
            _eventLog.log(_currentTick, io::MapCreated{width, height});
        }

        

        void spawnUnit(std::unique_ptr<IUnit> unit)
        {
            auto* unitPtr = unit.get();
            UnitId id = unitPtr->getId();
            Position pos = unitPtr->getPosition();
            
            _units.push_back(std::move(unit));
            _unitById[id] = unitPtr;
            _map.placeUnit(pos, id);
            
            _eventLog.log(_currentTick, io::UnitSpawned{
                id.value,
                unitPtr->getType(),
                pos.x,
                pos.y
            });
        }

        void setUnitMarchTarget(UnitId id, const Position& target)
        {
            auto* unit = findUnitById(id);
            if (!unit)
                return;

            unit->setMarchTarget(target);
            startMarch(id, unit->getPosition(), target);
        }

        // main simulation cycle
        void runSimulation()
        {
            while (shouldContinueSimulation())
            {
                _currentTick++;
                processTurn();
                removeDeadUnits();
            }
        }

    private:
        bool shouldContinueSimulation() const
        {
            size_t aliveCount = std::count_if(_units.begin(), _units.end(),
                [](const auto& unit) { return unit->canAct(); });
            
            return aliveCount > 1;
        }

        void processTurn()
        {
            // unit acts in order of creation?
            for (auto& unit : _units)
            {
                if (unit->canAct())
                {
                    unit->performAction(*this);
                }
            }
        }

        void removeDeadUnits()
        {
            for (auto it = _units.begin(); it != _units.end();)
            {
                if ((*it)->isDead())
                {
                    UnitId id = (*it)->getId();
                    Position pos = (*it)->getPosition();
                    
                    _map.removeUnit(pos, id);
                    _unitById.erase(id);
                    
                    _eventLog.log(_currentTick, io::UnitDied{id.value});
                    
                    it = _units.erase(it);
                }
                else
                {
                    ++it;
                }
            }
        }

    public:
        // IGameContext implementation
        uint32_t getMapWidth() const override { return _map.getWidth(); }
        uint32_t getMapHeight() const override { return _map.getHeight(); }
        
        bool isValidPosition(const Position& pos) const override
        {
            return _map.isValidPosition(pos);
        }

        IUnit* findUnitAt(const Position& pos) override
        {
            auto ids = _map.getAllUnitsAt(pos);
            for(auto id : ids){
                if(id.isValid() && !_unitById[id]->has_property(Unitproperty::Transparent)){
                    return _unitById[id];
                }
            }
            return nullptr;
        }

        std::vector<IUnit*> findUnitsAt(const Position& pos) override
        {
            std::vector<IUnit*> res;
            auto ids = _map.getAllUnitsAt(pos);
            for(auto id : ids){
                if(id.isValid() && !_unitById[id]->has_property(Unitproperty::Transparent)){
                    res.push_back(_unitById[id]);
                }
            }
            return res;
        }

        IUnit* findUnitById(UnitId id) override
        {
            auto it = _unitById.find(id);
            return (it != _unitById.end()) ? it->second : nullptr;
        }

        std::vector<IUnit*> findUnitsInRange(const Position& center, uint32_t range) override
        {
            std::vector<IUnit*> result;
            
            for (auto& unit : _units)
            {
                if (!unit->isDead() && !unit->has_property(Unitproperty::Transparent))
                {
                    Position unitPos = unit->getPosition();
                    if (unitPos.distanceTo(center) <= range && unitPos != center)
                    {
                        result.push_back(unit.get());
                    }
                }
            }
            
            return result;
        }

        std::vector<IUnit*> findAdjacentUnits(const Position& pos) override
        {
            return findUnitsInRange(pos, 1);
        }

        
        void moveUnit(UnitId unitId, const Position& newPos) override
        {
            auto* unit = findUnitById(unitId);
            if (!unit)
                return;

            Position oldPos = unit->getPosition();
            _map.moveUnit(oldPos, newPos, unitId);
            unit->setPosition(newPos);
            
            _eventLog.log(_currentTick, io::UnitMoved{
                unitId.value,
                newPos.x,
                newPos.y
            });
        }

        //start unit move event
        void startMarch(UnitId unitId, const Position& from, const Position& target) override
        {
            _eventLog.log(_currentTick, io::MarchStarted{
                unitId.value,
                from.x,
                from.y,
                target.x,
                target.y
            });
        }
        
        //end unit move event
        void endMarch(UnitId unitId, const Position& pos) override
        {
            _eventLog.log(_currentTick, io::MarchEnded{
                unitId.value,
                pos.x,
                pos.y
            });
        }    
    };
}