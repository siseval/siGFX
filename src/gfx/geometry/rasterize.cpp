#include "gfx/geometry/rasterize.h"


namespace gfx
{

void Rasterize::rasterize_filled_triangle(const BarycentricTriangle &triangle, std::vector<Vec2i> &pixels, const Vec2d clip_bounds)
{
    Box2d bounds {
        Vec2d {
            std::min({ triangle.v0.x, triangle.v1.x, triangle.v2.x }),
            std::min({ triangle.v0.y, triangle.v1.y, triangle.v2.y })
        }.round(),
        Vec2d {
            std::max({ triangle.v0.x, triangle.v1.x, triangle.v2.x }),
            std::max({ triangle.v0.y, triangle.v1.y, triangle.v2.y })
        }.round()
    };

    if (clip_bounds.x > 0.0 && clip_bounds.y > 0.0)
    {
        bounds.min.x = std::max(0.0, bounds.min.x);
        bounds.min.y = std::max(0.0, bounds.min.y);
        bounds.max.x = std::min((clip_bounds.x) - 1, bounds.max.x);
        bounds.max.y = std::min((clip_bounds.y) - 1, bounds.max.y);
    }

    int corners_inside = triangle.corners_inside(bounds);
    if (corners_inside == 4)
    {
        for (int y = bounds.min.y; y <= bounds.max.y; ++y)
        {
            for (int x = bounds.min.x; x <= bounds.max.x; ++x)
            {
                pixels.push_back(Vec2i { x, y });
            }
        }
        return;
    }

    double a = triangle.get_a();
    double b = triangle.get_b();
    double c = triangle.get_c();

    double d = triangle.get_d();
    double e = triangle.get_e();
    double f = triangle.get_f();

    double g = triangle.get_g();
    double h = triangle.get_h();
    double i = triangle.get_i();

    double area = triangle.get_area();

    if (area == 0.0)
    {
        return;
    }

    double start_x = static_cast<double>(bounds.min.x) + 0.5;
    double start_y = static_cast<double>(bounds.min.y) + 0.5;

    double w0_row = a * (bounds.min.x + 0.5) + b * (bounds.min.y + 0.5) + c;
    double w1_row = d * (bounds.min.x + 0.5) + e * (bounds.min.y + 0.5) + f;
    double w2_row = g * (bounds.min.x + 0.5) + h * (bounds.min.y + 0.5) + i;

    for (int y = bounds.min.y; y < bounds.max.y; ++y)
    {
        double w0 = w0_row;
        double w1 = w1_row;
        double w2 = w2_row;

        for (int x = bounds.min.x; x < bounds.max.x; ++x)
        {
            if ((w0 * w1 >= 0) && (w1 * w2 >= 0))
            {
                pixels.push_back(Vec2i { x, y });
            }

            w0 += a;
            w1 += d;
            w2 += g;
        }

        w0_row += b;
        w1_row += e;
        w2_row += h;
    }
}

// void Rasterize::rasterize_filled_triangle(const BarycentricTriangle &triangle, std::vector<Vec2i> &pixels, const Vec2d clip_bounds)
// {
//     Box2d bounds {
//         Vec2d {
//             std::min({ triangle.v0.x, triangle.v1.x, triangle.v2.x }),
//             std::min({ triangle.v0.y, triangle.v1.y, triangle.v2.y })
//         }.round(),
//         Vec2d {
//             std::max({ triangle.v0.x, triangle.v1.x, triangle.v2.x }),
//             std::max({ triangle.v0.y, triangle.v1.y, triangle.v2.y })
//         }.round()
//     };
//
//     if (clip_bounds.x > 0.0 && clip_bounds.y > 0.0)
//     {
//         bounds.min.x = std::max(0.0, bounds.min.x);
//         bounds.min.y = std::max(0.0, bounds.min.y);
//         bounds.max.x = std::min((clip_bounds.x) - 1, bounds.max.x);
//         bounds.max.y = std::min((clip_bounds.y) - 1, bounds.max.y);
//     }
//
//     int corners_inside = triangle.corners_inside(bounds);
//     if (corners_inside == 4)
//     {
//         for (int y = bounds.min.y; y <= bounds.max.y; ++y)
//         {
//             for (int x = bounds.min.x; x <= bounds.max.x; ++x)
//             {
//                 pixels.push_back(Vec2i { x, y });
//             }
//         }
//         return;
//     }
//
//     double a = triangle.get_a();
//     double b = triangle.get_b();
//     double c = triangle.get_c();
//
//     double d = triangle.get_d();
//     double e = triangle.get_e();
//     double f = triangle.get_f();
//
//     double g = triangle.get_g();
//     double h = triangle.get_h();
//     double i = triangle.get_i();
//
//     double area = triangle.get_area();
//
//     if (area == 0.0)
//     {
//         return;
//     }
//
//     constexpr int TILE_SIZE = 32;
//
//     int width  = bounds.max.x - bounds.min.x + 1;
//     int height = bounds.max.y - bounds.min.y + 1;
//     std::size_t resolution = static_cast<std::size_t>(width) * static_cast<std::size_t>(height);
//
//     struct Tile { Box2i box; };
//     std::vector<Tile> tiles;
//     for (int ty = bounds.min.y; ty <= bounds.max.y; ty += TILE_SIZE)
//     {
//         for (int tx = bounds.min.x; tx <= bounds.max.x; tx += TILE_SIZE)
//         {
//             Box2i tb;
//             tb.min.x = tx;
//             tb.min.y = ty;
//             tb.max.x = std::min(tx + TILE_SIZE, static_cast<int>(bounds.max.x) + 1);
//             tb.max.y = std::min(ty + TILE_SIZE, static_cast<int>(bounds.max.y) + 1);
//             tiles.push_back(Tile { tb });
//         }
//     }
//
//     auto rasterize_tile = [&](const Box2i &tb) {
//
//         double start_x = static_cast<double>(tb.min.x) + 0.5;
//         double start_y = static_cast<double>(tb.min.y) + 0.5;
//
//         double w0_row = a * (tb.min.x + 0.5) + b * (tb.min.y + 0.5) + c;
//         double w1_row = d * (tb.min.x + 0.5) + e * (tb.min.y + 0.5) + f;
//         double w2_row = g * (tb.min.x + 0.5) + h * (tb.min.y + 0.5) + i;
//
//         for (int y = tb.min.y; y < tb.max.y; ++y)
//         {
//             double w0 = w0_row;
//             double w1 = w1_row;
//             double w2 = w2_row;
//
//             for (int x = tb.min.x; x < tb.max.x; ++x)
//             {
//                 if ((w0 * w1 >= 0) && (w1 * w2 >= 0))
//                 {
//                     pixels.push_back(Vec2i { x, y });
//                 }
//
//                 w0 += a;
//                 w1 += d;
//                 w2 += g;
//             }
//
//             w0_row += b;
//             w1_row += e;
//             w2_row += h;
//         }
//     };
//
//     if (resolution > MIN_MULTITHREAD_PIXELS && tiles.size() > 1)
//     {
//         unsigned int hw = std::thread::hardware_concurrency();
//         unsigned int num_threads = hw ? hw : 2;
//         num_threads = static_cast<unsigned int>(std::min<std::size_t>(num_threads, tiles.size()));
//
//         std::vector<std::thread> threads;
//         threads.reserve(num_threads);
//
//         for (unsigned int ti = 0; ti < num_threads; ++ti)
//         {
//             threads.emplace_back([ti, num_threads, &tiles, &rasterize_tile]() {
//                 for (std::size_t idx = ti; idx < tiles.size(); idx += num_threads)
//                 {
//                     rasterize_tile(tiles[idx].box);
//                 }
//             });
//         }
//
//         for (auto &th : threads)
//         {
//             if (th.joinable()) th.join();
//         }
//     }
//     else
//     {
//         for (const auto &t : tiles)
//         {
//             rasterize_tile(t.box);
//         }
//     }
// }



}
