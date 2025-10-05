#pragma once

#include "../Core/BaseUnit.hpp"
#include "../Core/IGameContext.hpp"
#include "../Core/RandomUtils.hpp"

namespace sw::feature
{
    // attacks nearby enemies or moves towards the target
    class Swordsman : public core::BaseUnit
    {
    private:
        uint32_t _strength;

    public:
        Swordsman(core::UnitId id, core::Position pos, uint32_t health, uint32_t strength)
            : BaseUnit(id, pos, static_cast<int32_t>(health), true)
            , _strength(strength)
        {}

        std::string getType() const override
        {
            return "Swordsman";
        }

        void performAction(core::IGameContext& context) override
        {
            if (tryAttack(context))
                return;

            tryMoveTowardsTarget(context);
        }

    private:
        bool tryAttack(core::IGameContext& context)
        {
            auto adjacentUnits = context.findAdjacentUnits(_position);

            if (adjacentUnits.empty())
                return false;

            // select random target
            core::IUnit* target = core::RandomUtils::selectRandom(adjacentUnits);
            if (!target)
                return false;

            // attack
            context.dealDamage(_id, target->getId(), _strength);
            return true;
        }
    };
}