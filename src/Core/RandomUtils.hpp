#pragma once

#include <random>
#include <vector>
#include <algorithm>

namespace sw::core
{
    // utils for random generation
    class RandomUtils
    {
         inline static std::mt19937 _rng{std::random_device{}()};
    public:
        // select random element from vector
        template<typename T>
        static T* selectRandom(const std::vector<T*>& items)
        {
            if (items.empty())
                return nullptr;

            std::uniform_int_distribution<size_t> dist(0, items.size() - 1);
            return items[dist(_rng)];
        }
    };
}