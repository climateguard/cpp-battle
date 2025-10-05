#include "Core/GameWorld.hpp"
#include "Features/Swordsman.hpp"
#include "Features/Hunter.hpp"
#include "IO/System/CommandParser.hpp"
#include "IO/Commands/CreateMap.hpp"
#include "IO/Commands/SpawnSwordsman.hpp"
#include "IO/Commands/SpawnHunter.hpp"
#include "IO/Commands/March.hpp"

#include <fstream>
#include <iostream>
#include <memory>

using namespace sw;

int main(int argc, char* argv[])
{
    if (argc != 2)
	{
		throw std::runtime_error("Error: No file specified in command line argument");
	}

	std::ifstream file(argv[1]);
	if (!file)
	{
		throw std::runtime_error("Error: File not found - " + std::string(argv[1]));
	}

    core::GameWorld world;

    io::CommandParser parser;
    
    parser.add<io::CreateMap>([&world](io::CreateMap cmd)
    {
        world.createMap(cmd.width, cmd.height);
    });

    parser.add<io::SpawnSwordsman>([&world](io::SpawnSwordsman cmd)
    {
        auto unit = std::make_unique<feature::Swordsman>(
            core::UnitId{cmd.unitId},
            core::Position{cmd.x, cmd.y},
            cmd.hp,
            cmd.strength
        );
        world.spawnUnit(std::move(unit));
    });

    parser.add<io::SpawnHunter>([&world](io::SpawnHunter cmd)
    {
        auto unit = std::make_unique<feature::Hunter>(
            core::UnitId{cmd.unitId},
            core::Position{cmd.x, cmd.y},
            cmd.hp,
            cmd.agility,
            cmd.strength,
            cmd.range
        );
        world.spawnUnit(std::move(unit));
    });

    parser.add<io::March>([&world](io::March cmd)
    {
        world.setUnitMarchTarget(
            core::UnitId{cmd.unitId},
            core::Position{cmd.targetX, cmd.targetY}
        );
    });

    try
    {
        parser.parse(file);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error parsing commands: " << e.what() << std::endl;
        return 1;
    }


    world.runSimulation();

    return 0;
}