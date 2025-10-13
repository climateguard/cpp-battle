#pragma once
#include <string>
namespace sw::core
{
class IComponent
    {
    public:
        virtual ~IComponent() = default;
        virtual std::string getTypeName() const = 0;
    };
}