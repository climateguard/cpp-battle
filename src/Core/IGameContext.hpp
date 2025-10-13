#pragma once

#include "UnitId.hpp"
#include "Position.hpp"
#include <vector>
#include <cstdint>
#include "../IO/System/EventLog.hpp"

namespace sw::core
{
    class IUnit;
    // Context interface for units to access the game world
    class IGameContext
    {
    public:
        virtual ~IGameContext() = default;

        // game map info
        virtual uint32_t getMapWidth() const = 0;
        virtual uint32_t getMapHeight() const = 0;
        virtual bool isValidPosition(const Position& pos) const = 0;
        // Unit search and etc
        virtual IUnit* findUnitAt(const Position& pos) = 0;
        virtual IUnit* findUnitById(UnitId id) = 0;
        virtual std::vector<IUnit*> findUnitsAt(const Position& pos) = 0;
        virtual std::vector<IUnit*> findUnitsInRange(const Position& center, uint32_t range) = 0;
        virtual std::vector<IUnit*> findAdjacentUnits(const Position& pos) = 0;

        // Actions
        virtual void moveUnit(UnitId unitId, const Position& newPos) = 0;
        
        // Movenment  events
        virtual void startMarch(UnitId unitId, const Position& from, const Position& target) = 0;
        virtual void endMarch(UnitId unitId, const Position& pos) = 0;

        virtual uint64_t get_tick() const = 0;
        virtual EventLog& getEventLog() = 0;

        template<typename Event>
        void logEvent(const Event& event)
        {
            getEventLog().log(get_tick(), event);
        }
    };
}