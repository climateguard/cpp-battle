#pragma once

#include "UnitId.hpp"
#include "Position.hpp"
#include "IAction.hpp"
#include <string>
#include <memory>

namespace sw::core
{
    class IGameContext;

    // Base unit interface
    class IUnit
    {
    public:
        virtual ~IUnit() = default;
        // Basic properties
        virtual UnitId getId() const = 0;
        virtual std::string getType() const = 0;
        virtual Position getPosition() const = 0;
        virtual void setPosition(const Position& pos) = 0;

        // characteristics
        virtual bool hasHealth() const = 0;
        virtual int32_t getHealth() const = 0;
        virtual void takeDamage(int32_t damage) = 0;
        
        // Possibilities
        virtual bool canAct() const = 0;

        // target to movenment control
        virtual void setMarchTarget(const Position& target) = 0;
        virtual void clearMarchTarget() = 0;
        virtual bool hasMarchTarget() const = 0;
        virtual Position getMarchTarget() const = 0;
        
        // actions
        virtual void performAction(IGameContext& context) = 0;
        virtual void addAction(std::unique_ptr<IAction> action) = 0;
        virtual const std::vector<std::unique_ptr<IAction>>& getActions() const = 0;
        
        // condition
        virtual bool isDead() const = 0;
    };
}