#pragma once

#include "IUnit.hpp"
#include <algorithm>
#include "IAction.hpp"
#include "../Features/Component/HealthComponent.hpp"
namespace sw::core
{
    // Base unit implemetation
    class BaseUnit : public IUnit
    {
    protected:
        UnitId _id;
        Position _position; //current position
        
        
        //target to move
        bool _hasTarget{false};
        Position _targetPosition;
        std::unordered_map<std::type_index, std::unique_ptr<IComponent>> _components;
        std::vector<std::unique_ptr<IAction>> _actions;
    public:
        BaseUnit(UnitId id, Position pos)
            : _id(id), _position(pos)
        {}

        // IUnit implementation
        UnitId getId() const override { return _id; }
        Position getPosition() const override { return _position; }
        void setPosition(const Position& pos) override { _position = pos; }
        
        bool isDead() const override
        {
            if (auto* health = getComponent<sw::feature::HealthComponent>())
                return health->getCurrent() <= 0;
            return false; // no health comp no dead
        }

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

    void addComponent(std::unique_ptr<IComponent> component) override
        {
            auto typeIdx = std::type_index(typeid(*component));
            _components[typeIdx] = std::move(component);
        }

    protected:
        IComponent* getComponentByType(std::type_index type) override
        {
            auto it = _components.find(type);
            return (it != _components.end()) ? it->second.get() : nullptr;
        }
        
        const IComponent* getComponentByType(std::type_index type) const override
        {
            auto it = _components.find(type);
            return (it != _components.end()) ? it->second.get() : nullptr;
        }
        
        void removeComponentByType(std::type_index type) override
        {
            _components.erase(type);
        }
    };
}