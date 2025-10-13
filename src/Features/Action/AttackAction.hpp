#pragma once

#include "../../Core/IAction.hpp"
#include "../../Core/IUnit.hpp"
#include "../../Core/IGameContext.hpp"
#include "../../Core/Position.hpp"
#include "../../Core/RandomUtils.hpp"
#include <vector>
#include "../../IO/Events/UnitAttacked.hpp"


namespace sw::feature::action {

//universal attack. Supports melee and ranged combat
class AttackAction : public sw::core::IAction {
    uint32_t _damage;
    uint32_t _minRange;
    uint32_t _maxRange;
    bool _skipIfEnemiesNearby;
public:
    AttackAction(uint32_t damage,
                 uint32_t minRange,
                 uint32_t maxRange,
                 bool skipIfEnemiesNearby = false)
        : _damage(damage),
          _minRange(minRange),
          _maxRange(maxRange),
          _skipIfEnemiesNearby(skipIfEnemiesNearby) {}

    bool execute(sw::core::IUnit& self, sw::core::IGameContext& ctx) override {
        if (self.isDead() || _damage == 0)
            return false;

        const auto& pos = self.getPosition();

        // если нужно — пропускаем атаку, если рядом враги
        if (_skipIfEnemiesNearby) {
            auto adj = ctx.findUnitsInRange(pos, 1);
            for (auto* u : adj)
                if (u && !u->isDead() && u->getId() != self.getId())
                    return false;
        }

        // выбираем цели в диапазоне [minRange, maxRange]
        std::vector<sw::core::IUnit*> targets;
        auto all = ctx.findUnitsInRange(pos, _maxRange);
        for (auto* u : all) {
            if (!u || u->isDead() || u->getId() == self.getId())
                continue;

            uint32_t dist = static_cast<uint32_t>(pos.distanceTo(u->getPosition()));
            if (dist >= _minRange && dist <= _maxRange)
                targets.push_back(u);
        }

        if (targets.empty())
            return false;

        auto* target = sw::core::RandomUtils::selectRandom(targets);
         if (!target->hasHealth())
                return false;
        target->takeDamage(_damage);
        ctx.logEvent(io::UnitAttacked{
                self.getId().value,
                target->getId().value,
                _damage,
                static_cast<uint32_t>(std::max(0, target->getHealth()))
            });
        return true;
    }
};

} // namespace sw::features::action
