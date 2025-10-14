#pragma once

#include "../Core/BaseUnit.hpp"
#include "../Core/IGameContext.hpp"
#include "../Core/RandomUtils.hpp"
#include "Action/AttackAction.hpp"
#include "Action/MoveAction.hpp"
#include <algorithm>
#include "Component/HealthComponent.hpp"
namespace sw::feature
{
    // shoots from a distance or attacks in close combat
    class Hunter : public core::BaseUnit
    {
    private:
        uint32_t _agility;   // Ranged attack damage
        uint32_t _strength;  // Melee attack damage
        uint32_t _range;     // shoots range

    public:
        Hunter(core::UnitId id, core::Position pos, uint32_t health, 
               uint32_t agility, uint32_t strength, uint32_t range)
            : BaseUnit(id, pos)
            , _agility(agility)
            , _strength(strength)
            , _range(range)
        {
             using namespace sw::feature::action;
        addComponent(std::make_unique<HealthComponent>(health));
        // ranged attack
        addAction(std::make_unique<AttackAction>(_agility, 2, _range, true));

        // melee attack
        addAction(std::make_unique<AttackAction>(_strength, 1, 1));

        // move
        addAction(std::make_unique<MoveAction>());
        }

        std::string getType() const override
        {
            return "Hunter";
        }
    };
}