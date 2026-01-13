#include "gfx/core/types/uuid.h"

namespace gfx
{

uint64_t part1;
uint64_t part2;

UUID::UUID() : UUID(generate()) {}
UUID::UUID(const uint64_t p1, const uint64_t p2) : part1(p1), part2(p2) {}

bool UUID::operator==(const UUID &other) const
{
    return part1 == other.part1 && part2 == other.part2;
}

bool UUID::operator!=(const UUID &other) const
{
    return !(*this == other);
}

UUID UUID::generate()
{
    static uint64_t counter = 0;
    UUID id(0, 0);
    id.part1 = static_cast<uint64_t>(std::rand()) << 32 | static_cast<uint64_t>(std::rand());
    id.part2 = static_cast<uint64_t>(std::rand()) << 32 | static_cast<uint64_t>(std::rand());
    id.part2 ^= counter++;
    return id;
}

}
