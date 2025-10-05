#pragma once

#include "../Core/BaseUnit.hpp"
#include "../Core/IGameContext.hpp"
#include "../Core/RandomUtils.hpp"
#include <algorithm>

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
            : BaseUnit(id, pos, static_cast<int32_t>(health), true)
            , _agility(agility)
            , _strength(strength)
            , _range(range)
        {}

        std::string getType() const override
        {
            return "Hunter";
        }

        void performAction(core::IGameContext& context) override
        {
            if (tryRangedAttack(context))
                return;

            if (tryMeleeAttack(context))
                return;

            tryMoveTowardsTarget(context);
        }

    private:
        bool tryRangedAttack(core::IGameContext& context)
        {
            // can only shoot if there are no other units in the adjacent cells
            auto adjacentUnits = context.findAdjacentUnits(_position);
            if (!adjacentUnits.empty())
                return false;

            // find targets at a distance from 2 to Range
            std::vector<core::IUnit*> targets;
            auto unitsInRange = context.findUnitsInRange(_position, _range);
            
            for (auto* unit : unitsInRange)
            {
                if (unit->getId() == _id)
                    continue;

                uint32_t dist = _position.distanceTo(unit->getPosition());
                if (dist >= 2 && dist <= _range)
                {
                    targets.push_back(unit);
                }
            }

            if (targets.empty())
                return false;

            // Select a random target
            core::IUnit* target = core::RandomUtils::selectRandom(targets);
            if (!target)
                return false;

            // shoot
            context.dealDamage(_id, target->getId(), _agility);
            return true;
        }

        bool tryMeleeAttack(core::IGameContext& context)
        {
            auto adjacentUnits = context.findAdjacentUnits(_position);

            if (adjacentUnits.empty())
                return false;

            // Select a random target
            core::IUnit* target = core::RandomUtils::selectRandom(adjacentUnits);
            if (!target)
                return false;

            // attack
            context.dealDamage(_id, target->getId(), _strength);
            return true;
        }
    };
}