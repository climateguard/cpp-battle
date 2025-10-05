#pragma once

#include <cstdint>
#include <functional>

namespace sw::core
{
    // Identificator of unit
    struct UnitId
    {
        uint32_t value{};

        explicit UnitId(uint32_t val = 0) : value(val) {}

        bool operator==(const UnitId& other) const { return value == other.value; }
        bool operator!=(const UnitId& other) const { return value != other.value; }
        bool operator<(const UnitId& other) const { return value < other.value; }
        
        bool isValid() const { return value != 0; }
    };
}

// for use UnitId in hash table
namespace std
{
    template<>
    struct hash<sw::core::UnitId>
    {
        size_t operator()(const sw::core::UnitId& id) const
        {
            return hash<uint32_t>()(id.value);
        }
    };
}