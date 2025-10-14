#pragma once

#include "UnitId.hpp"
#include "Position.hpp"
#include <vector>
#include <cstdint>
#include "../IO/System/EventLog.hpp"

namespace sw::core
{
    class IUnit;
    class GameMap;
    // Context interface for units to access the game world
    class IGameContext
    {
    public:
        virtual ~IGameContext() = default;

        virtual GameMap& getMap() = 0;

        // Actions
        virtual void moveUnit(UnitId unitId, const Position& newPos) = 0; 

        virtual uint64_t get_tick() const = 0;
        virtual EventLog& getEventLog() = 0;

        template<typename Event>
        void logEvent(const Event& event)
        {
            getEventLog().log(get_tick(), event);
        }
    };
}