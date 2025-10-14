#pragma once

#include "../Core/BaseUnit.hpp"
#include "../Core/IGameContext.hpp"
#include "../Core/RandomUtils.hpp"
#include "Action/AttackAction.hpp"
#include "Action/MoveAction.hpp"
#include "Component/HealthComponent.hpp"
namespace sw::feature {

// attacks nearby enemies or moves towards the target
class Swordsman : public sw::core::BaseUnit {
    uint32_t _strength;

public:
    Swordsman(sw::core::UnitId id,
              sw::core::Position pos,
              uint32_t health,
              uint32_t strength)
        : BaseUnit(id, pos),
          _strength(strength)
    {
        using namespace sw::feature::action;
        addComponent(std::make_unique<HealthComponent>(health));
        // near attack
        addAction(std::make_unique<AttackAction>(_strength, 1, 1));

        // if cant attack
        addAction(std::make_unique<MoveAction>());
    }

    std::string getType() const override { return "Swordsman"; }
};

} // namespace sw::features
