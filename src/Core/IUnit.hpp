#pragma once

#include "UnitId.hpp"
#include "Position.hpp"
#include "IAction.hpp"
#include <string>
#include <memory>
#include <set>
#include "IComponent.hpp"

namespace sw::core
{
    enum class Unitproperty {
        Transparent, //Can't be attacked
        NotOccupy //Not occupy map field
    };
    class IGameContext;

    // Base unit interface
    class IUnit
    {
    protected:
        std::set<Unitproperty> _properties;
    public:
        virtual ~IUnit() = default;
        // Basic properties
        virtual UnitId getId() const = 0;
        virtual std::string getType() const = 0;
        virtual Position getPosition() const = 0;
        virtual void setPosition(const Position& pos) = 0;

        // Possibilities
        virtual bool canAct() const = 0;
        virtual bool has_property(Unitproperty pr){
            return _properties.count(pr);
        }
        virtual void add_property(Unitproperty pr) {
            _properties.insert(pr);
        }
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

        //Components
        virtual void addComponent(std::unique_ptr<IComponent> component) = 0;
        
        template<typename T>
        T* getComponent()
        {
            return static_cast<T*>(getComponentByType(typeid(T)));
        }
        
        template<typename T>
        const T* getComponent() const
        {
            return static_cast<const T*>(getComponentByType(typeid(T)));
        }
        
        template<typename T>
        bool hasComponent() const
        {
            return getComponent<T>() != nullptr;
        }
        
        template<typename T>
        void removeComponent()
        {
            removeComponentByType(typeid(T));
        }

    protected:
        virtual IComponent* getComponentByType(std::type_index type) = 0;
        virtual const IComponent* getComponentByType(std::type_index type) const = 0;
        virtual void removeComponentByType(std::type_index type) = 0;
    };
}