#pragma once

#include <string>
#include <cstddef>

namespace gfx
{

bool decode_utf8(const std::string &s, size_t pos, uint32_t &out_codepoint, size_t &bytes);

}
