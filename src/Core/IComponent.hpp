#pragma once
#include <string>
namespace sw::core
{
//Base interface for unit encapsulated characteristics into components. 
class IComponent
    {
    public:
        virtual ~IComponent() = default;
        virtual std::string getTypeName() const = 0;
    };
}