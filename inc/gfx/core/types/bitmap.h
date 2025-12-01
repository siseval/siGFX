#pragma once

#include <fstream>
#include <stdexcept>
#include <vector>
#include <algorithm>

#include "gfx/core/types/color4.h"
#include "gfx/math/vec2.h"

namespace gfx
{

class Bitmap
{

public:

    Bitmap(const Vec2i resolution)
        : resolution(resolution), pixels(std::vector<Color4>(resolution.x * resolution.y, Color4 { 0, 0, 0, 255 })) 
    {
        if (resolution.x <= 0 || resolution.y <= 0)
        {
            this->resolution = { 1, 1 };
            pixels.resize(1, Color4 { 0, 0, 0, 255 });
        }
    }

    void resize(const Vec2i new_resolution)
    {
        resolution = new_resolution;
        pixels.resize(resolution.x * resolution.y, Color4 { 0, 0, 0, 255 });
    }

    void set_pixel(const Vec2i pos, const Color4 color)
    {
        if (pos.x < 0 || pos.y < 0 || pos.x >= resolution.x || pos.y >= resolution.y)
        {
            return;
        }
        pixels[pos.x + pos.y * resolution.x] = color;
    }

    Color4 get_pixel(const Vec2i pos) const
    {
        if (pos.x < 0 || pos.y < 0 || pos.x >= resolution.x || pos.y >= resolution.y)
        {
            return Color4 { 0, 0, 0, 255 };
        }
        return pixels[pos.x + pos.y * resolution.x];
    }

    void fill(const Color4 color = Color4 { 0, 0, 0, 255 })
    {
        std::fill(pixels.begin(), pixels.end(), color);
    }

    void compress_colors(const std::vector<Color4>& palette)
    {
        for(auto& pixel : pixels)
        {
            auto it = std::min_element(palette.begin(), palette.end(), [&pixel](const Color4& a, const Color4& b) {
                return Color4::distance(a, pixel) < Color4::distance(b, pixel);
            });
            if(it != palette.end())
            {
                pixel = *it;
            }
        }
    }

    static Bitmap decode_bmp(const std::string& filename)
    {
        std::ifstream file{ filename, std::ios::binary };
        if(!file)
        {
            throw std::runtime_error{ "Failed to open BMP file: " + filename };
        }

        // BMP Header
        uint16_t file_type{};
        file.read(reinterpret_cast<char*>(&file_type), 2);
        if(file_type != 0x4D42) // 'BM'
        {
            throw std::runtime_error{ "Not a valid BMP file" };
        }

        file.ignore(8); // Skip file size and reserved fields

        uint32_t data_offset{};
        file.read(reinterpret_cast<char*>(&data_offset), 4);

        // DIB Header
        uint32_t dib_header_size{};
        file.read(reinterpret_cast<char*>(&dib_header_size), 4);

        if(dib_header_size < 40)
        {
            throw std::runtime_error{ "Unsupported BMP format: DIB header too small" };
        }

        int32_t width{}, height{};
        file.read(reinterpret_cast<char*>(&width), 4);
        file.read(reinterpret_cast<char*>(&height), 4);

        uint16_t planes{}, bit_count{};
        file.read(reinterpret_cast<char*>(&planes), 2);
        file.read(reinterpret_cast<char*>(&bit_count), 2);

        uint32_t compression{};
        file.read(reinterpret_cast<char*>(&compression), 4);

        // Skip image size, resolution, and color table entries (16 bytes)
        file.ignore(16);

        // Skip rest of header if longer than 40 bytes
        uint32_t extra_header_bytes = dib_header_size > 40 ? dib_header_size - 40 : 0;
        if(extra_header_bytes > 0)
        {
            file.ignore(extra_header_bytes);
        }

        // Skip color masks if using BI_BITFIELDS
        if(compression == 3) // BI_BITFIELDS
        {
            // Bitfields are 3 DWORDs for RGB, sometimes 4 for RGBA (depends on header)
            uint32_t mask_count = (dib_header_size >= 56) ? 4 : 3;
            file.ignore(mask_count * 4);
        }

        if(planes != 1 || (bit_count != 24 && bit_count != 32))
        {
            throw std::runtime_error{ "Unsupported BMP: Only 24-bit or 32-bit images supported" };
        }

        if(compression != 0 && compression != 3)
        {
            throw std::runtime_error{ "Unsupported BMP: Compressed BMPs not supported" };
        }

        // Go to pixel data
        file.seekg(data_offset, std::ios::beg);

        int row_padded = ((bit_count * width + 31) / 32) * 4;
        int bytes_per_pixel = bit_count / 8;

        Bitmap bmp{ Vec2i{ width, std::abs(height) } };
        bool flip_vertically{ height > 0 };

        std::vector<uint8_t> row_data(row_padded);

        for(int y = 0; y < std::abs(height); ++y)
        {
            file.read(reinterpret_cast<char*>(row_data.data()), row_padded);
            int bmp_y = flip_vertically ? (std::abs(height) - 1 - y) : y;

            for(int x = 0; x < width; ++x)
            {
                int idx = x * bytes_per_pixel;
                uint8_t b = row_data[idx];
                uint8_t g = row_data[idx + 1];
                uint8_t r = row_data[idx + 2];
                uint8_t a = (bit_count == 32) ? row_data[idx + 3] : 255;

                Color4 color{ r, g, b, a };
                bmp.set_pixel(Vec2i{ x, bmp_y }, color);
            }
        }

        return bmp;
    }

    Vec2i resolution;
    std::vector<Color4> pixels;

};

}
