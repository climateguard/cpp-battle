#pragma once

#include "IUnit.hpp"
#include <algorithm>

namespace sw::core
{
    // Base unit implemetation
    class BaseUnit : public IUnit
    {
    protected:
        UnitId _id;
        Position _position; //current position
        int32_t _health{0};
        bool _movable{true};  //static or movable unit
        
        //target to move
        bool _hasTarget{false};
        Position _targetPosition;

    public:
        BaseUnit(UnitId id, Position pos, int32_t health, bool movable = true)
            : _id(id), _position(pos), _health(health), _movable(movable)
        {}

        // IUnit implementation
        UnitId getId() const override { return _id; }
        Position getPosition() const override { return _position; }
        void setPosition(const Position& pos) override { _position = pos; }
        
        bool hasHealth() const override { return true; }
        int32_t getHealth() const override { return _health; }
        
        void takeDamage(int32_t damage) override
        {
            _health -= damage;
        }
        
        bool canMove() const override { return _movable; }
        bool isDead() const override { return _health <= 0; }
        bool canAct() const override { return !isDead(); }

        void setMarchTarget(const Position& target)
        {
            _hasTarget = true;
            _targetPosition = target;
        }

        void clearMarchTarget()
        {
            _hasTarget = false;
        }

        bool hasMarchTarget() const
        {
            return _hasTarget;
        }

        Position getMarchTarget() const
        {
            return _targetPosition;
        }

    protected:
        // Attempts to move the unit one step towards its march target
        bool tryMoveTowardsTarget(IGameContext& context)
        {
            if (!_hasTarget || _position == _targetPosition)
            {
                if (_hasTarget)
                {
                    context.endMarch(_id, _position);
                    clearMarchTarget();
                }
                return false;
            }

            Position nextPos = calculateNextStep(_position, _targetPosition);
            
            if (!context.isValidPosition(nextPos) || context.findUnitAt(nextPos) != nullptr)
            {
                return false; // can't move
            }

            context.moveUnit(_id, nextPos);
            
            //target achive
            if (_position == _targetPosition)
            {
                context.endMarch(_id, _position);
                clearMarchTarget();
            }
            
            return true;
        }

        // Calculate next step position
        Position calculateNextStep(const Position& from, const Position& to) const
        {
            Position next = from;
            
            if (from.x < to.x) next.x++;
            else if (from.x > to.x) next.x--;
            
            if (from.y < to.y) next.y++;
            else if (from.y > to.y) next.y--;
            
            return next; 
        }
    };
}