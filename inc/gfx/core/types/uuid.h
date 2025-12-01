#pragma once

#include <cstdlib>
#include <cstdint>
#include <functional>

namespace gfx
{

struct UUID
{
    uint64_t part1;
    uint64_t part2;

    UUID() : UUID(generate()) {}
    UUID(const uint64_t p1, const uint64_t p2) : part1(p1), part2(p2) {}

    static UUID generate()
    {
        static uint64_t counter = 0;
        UUID id(0, 0);
        id.part1 = static_cast<uint64_t>(std::rand()) << 32 | static_cast<uint64_t>(std::rand());
        id.part2 = static_cast<uint64_t>(std::rand()) << 32 | static_cast<uint64_t>(std::rand());
        id.part2 ^= counter++;
        return id;
    }

    bool operator==(const UUID &other) const
    {
        return part1 == other.part1 && part2 == other.part2;
    }

    bool operator!=(const UUID &other) const
    {
        return !(*this == other);
    }
};

}

template <>
struct std::hash<gfx::UUID>
{
    size_t operator()(const gfx::UUID &uuid) const
    {
        return std::hash<uint64_t>()(uuid.part1) ^ (std::hash<uint64_t>()(uuid.part2) << 1);
    }
};
