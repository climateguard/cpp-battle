#pragma once
#include "../../Core/IComponent.hpp"

namespace sw::feature{
    
    class HealthComponent : public sw::core::IComponent
    {
    private:
        int32_t _current;
        
    public:
        HealthComponent(int32_t maxHealth)
            : _current(maxHealth)
        {}
        
        std::string getTypeName() const override { return "Health"; }
        
        int32_t getCurrent() const { return _current; }
        
        void takeDamage(int32_t amount)
        {
            _current -= amount;
            if (_current < 0) _current = 0;
        }
        
    };

};
