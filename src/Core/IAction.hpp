#pragma once
#include "IGameContext.hpp"
#include "IUnit.hpp"

namespace sw::core {
//Action inteface
class IAction {
public:
    virtual ~IAction() = default;
    virtual bool execute(IUnit& self, IGameContext& ctx) = 0;
};

}