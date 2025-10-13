#pragma once

#include "IUnit.hpp"
#include <algorithm>
#include "IAction.hpp"

namespace sw::core
{
    // Base unit implemetation
    class BaseUnit : public IUnit
    {
    protected:
        UnitId _id;
        Position _position; //current position
        int32_t _health{0};
        
        //target to move
        bool _hasTarget{false};
        Position _targetPosition;
        std::vector<std::unique_ptr<IAction>> _actions;
    public:
        BaseUnit(UnitId id, Position pos, int32_t health)
            : _id(id), _position(pos), _health(health)
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
        
        bool isDead() const override { return _health <= 0; }
        bool canAct() const override { return !isDead(); } 

        void setMarchTarget(const Position& target) override
        {
            _hasTarget = true;
            _targetPosition = target;
        }

        void clearMarchTarget() override
        {
            _hasTarget = false;
        }

        bool hasMarchTarget() const override
        {
            return _hasTarget;
        }

        Position getMarchTarget() const override
        {
            return _targetPosition;
        }

         void addAction(std::unique_ptr<IAction> action) override {
                _actions.push_back(std::move(action));
            }

    const std::vector<std::unique_ptr<IAction>>& getActions() const override {
        return _actions;
    }

    void performAction(IGameContext& ctx) override {
        if (isDead()) return;
        for (auto& act : _actions) {
            if(act->execute(*this, ctx))
                break;
        }
    } 
    };
}