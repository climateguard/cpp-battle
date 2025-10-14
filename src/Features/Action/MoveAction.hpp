#pragma once

#include "../../Core/IAction.hpp"
#include "../../Core/Position.hpp"
#include "../../Core/IUnit.hpp"
#include "../../Core/IGameContext.hpp"
#include "../../IO/Events/MarchEnded.hpp"
namespace sw::feature::action {

//move unit on map
class MoveAction : public sw::core::IAction {

public:
    explicit MoveAction()
         {}

    bool execute(sw::core::IUnit& self, sw::core::IGameContext& ctx) override {
        sw::core::GameMap gmap = ctx.getMap();
        if (self.isDead())
            return false;
        auto _target = self.getMarchTarget();
        const auto current = self.getPosition();
        if (current == _target){
            ctx.logEvent(io::MarchEnded{
                self.getId().value,
                self.getPosition().x,
                self.getPosition().y
            });
            return false; // already at
        }
        
        
        auto next = current.stepTowards(_target);

        // free position?
        if (!gmap.isValidPosition(next) || gmap.findUnitAt(next) != nullptr)
            return false;

        // execute move
        ctx.moveUnit(self.getId(), next);
        return true;
    }
};

} // namespace sw::features::action
