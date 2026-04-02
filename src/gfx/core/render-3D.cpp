#include "gfx/core/render-3D.h"

#include "gfx/math/box2.h"

#include <chrono>
#include <map>
#include <cmath>
#include <algorithm>
#include <thread>

namespace gfx
{

Render3D::Render3D(std::shared_ptr<RenderSurface> surface)
    : ambient_light(0.0),
      scene_graph(std::make_shared<SceneGraph3D>()),
      surface(surface),
      thread_pool(std::make_shared<ThreadPool>(std::thread::hardware_concurrency())) {}


void Render3D::draw_frame() const
{
    double t {
        std::chrono::duration<double, std::milli>(
            std::chrono::high_resolution_clock::now().time_since_epoch()
        ).count() / 1000.0
    };

    for (auto &shader : fullscreen_shaders)
    {
        shader->set_uniforms(
            Shader3D::FragUniforms {
                .t = t,
                .light_dir = light_dir,
                .ambient_intensity = ambient_light,
                .near_plane = get_camera().get_z_near(),
                .far_plane = get_camera().get_z_far()
            }
        );
    }

    std::map<int, Shader3D> shader_map;

    std::vector<ScreenTriangle> screen_triangles {
        generate_screen_triangles(shader_map)
    };

    std::vector<Tile> tiles {
        generate_tiles()
    };

    bin_triangles(screen_triangles, tiles);

    // for (auto &tile : tiles)
    // {
    //     render_tile(tile, shader_map, t);
    // }

    thread_pool->run(static_cast<int>(tiles.size()), [&](const int tile_index) {
        render_tile(tiles[tile_index], shader_map, t);
    });
}


std::vector<Render3D::ScreenTriangle> Render3D::generate_screen_triangles(std::map<int, Shader3D> &shader_map) const
{
    std::vector<ScreenTriangle> screen_triangles;

    const Vec2i resolution { surface->get_resolution() };

    const double aspect_ratio {
        static_cast<double>(resolution.x) /
        static_cast<double>(resolution.y)
    };

    const Camera &camera { get_camera() };
    const Matrix4x4d view_matrix { camera.get_view_matrix() };
    const Matrix4x4d projection_matrix { camera.get_projection_matrix(aspect_ratio) };

    for (const auto &[primitive, transform] : scene_graph->get_draw_queue())
    {
        const PolygonMesh &mesh { primitive->get_mesh() };

        const Shader3D shader { primitive->get_shader() };

        shader.get_vertex_shader()->set_uniforms(
            Shader3D::VertUniforms {
                .model_matrix = transform,
                .view_matrix = view_matrix,
                .projection_matrix = projection_matrix,
                .mvp_matrix = projection_matrix * view_matrix * transform
            }
        );

        shader_map[shader.get_id()] = shader;

        for (size_t i = 0; i < mesh.get_indices().size(); i += 3)
        {
            const size_t idx0 { mesh.get_indices()[i] };
            const size_t idx1 { mesh.get_indices()[i + 1] };
            const size_t idx2 { mesh.get_indices()[i + 2] };

            std::array<Shader3D::VertOutput, 3> tri_out;

            auto model_to_clip {
                [&](int src_index, int dest_index) {
                    Shader3D::VertInput vert_in {
                        .pos = mesh.get_vertices()[src_index],
                        .normal = mesh.get_normals()[src_index],
                    };
                    tri_out[dest_index] = shader.vert(vert_in);
                }
            };

            model_to_clip(idx0, 0);
            model_to_clip(idx1, 1);
            model_to_clip(idx2, 2);

            if (tri_out[0].w <= 0 || tri_out[1].w <= 0 || tri_out[2].w <= 0)
            {
                continue;
            }

            std::array<ScreenVertex, 3> screen_vertices;
            auto clip_to_screen {
                [&](const int source_index, const int dest_index) {
                    const auto &clip { tri_out[source_index].xyz };

                    const double inv_w { 1.0f / static_cast<float>(tri_out[source_index].w) };
                    const Vec3d ndc { clip * inv_w };

                    screen_vertices[dest_index].pos = {
                        (ndc.x * 0.5 + 0.5) * resolution.x,
                        (1.0 - (ndc.y * 0.5 + 0.5)) * resolution.y
                    };

                    screen_vertices[dest_index].normal = tri_out[source_index].normal;

                    screen_vertices[dest_index].z_over_w = ndc.z;
                    screen_vertices[dest_index].inv_w = inv_w;
                }
            };

            clip_to_screen(0, 0);
            clip_to_screen(1, 1);
            clip_to_screen(2, 2);

            const float area {
                Vec2f::cross(
                    screen_vertices[1].pos - screen_vertices[0].pos,
                    screen_vertices[2].pos - screen_vertices[0].pos
                )
            };

            if (area <= 0.0f)
            {
                continue;
            }

            screen_vertices[0].color = mesh.get_colors()[idx0];
            screen_vertices[1].color = mesh.get_colors()[idx1];
            screen_vertices[2].color = mesh.get_colors()[idx2];

            screen_triangles.emplace_back(
                std::move(screen_vertices[0]),
                std::move(screen_vertices[1]),
                std::move(screen_vertices[2]),
                shader.get_id()
            );
        }
    }
    return screen_triangles;
}

std::vector<Render3D::Tile> Render3D::generate_tiles() const
{
    const Vec2i resolution { surface->get_resolution() };

    std::vector<Tile> tiles;

    for (int ty = 0; ty < resolution.y; ty += TILE_SIZE)
    {
        for (int tx = 0; tx < resolution.x; tx += TILE_SIZE)
        {
            tiles.emplace_back(Vec2i { tx, ty });
        }
    }

    return tiles;
}

void Render3D::bin_triangles(const std::vector<ScreenTriangle> &screen_triangles, std::vector<Tile> &tiles) const
{
    const Vec2i resolution { surface->get_resolution() };

    for (const auto &triangle : screen_triangles)
    {
        const Vec2i v0_tile {
            static_cast<int>(std::floor(triangle.v0.pos.x)) / TILE_SIZE,
            static_cast<int>(std::floor(triangle.v0.pos.y)) / TILE_SIZE
        };
        const Vec2i v1_tile {
            static_cast<int>(std::floor(triangle.v1.pos.x)) / TILE_SIZE,
            static_cast<int>(std::floor(triangle.v1.pos.y)) / TILE_SIZE
        };
        const Vec2i v2_tile {
            static_cast<int>(std::floor(triangle.v2.pos.x)) / TILE_SIZE,
            static_cast<int>(std::floor(triangle.v2.pos.y)) / TILE_SIZE
        };

        const Box2i tile_bounds {
            Vec2i {
                std::max(0, std::min({ v0_tile.x, v1_tile.x, v2_tile.x })),
                std::max(0, std::min({ v0_tile.y, v1_tile.y, v2_tile.y }))
            },
            Vec2i {
                std::min((resolution.x + TILE_SIZE - 2) / TILE_SIZE - 1, std::max({ v0_tile.x, v1_tile.x, v2_tile.x })),
                std::min((resolution.y + TILE_SIZE - 2) / TILE_SIZE - 1, std::max({ v0_tile.y, v1_tile.y, v2_tile.y }))
            }
        };

        for (int ty = tile_bounds.min.y; ty <= tile_bounds.max.y; ++ty)
        {
            for (int tx = tile_bounds.min.x; tx <= tile_bounds.max.x; ++tx)
            {
                const int tile_index { ty * ((resolution.x + TILE_SIZE - 1) / TILE_SIZE) + tx };

                tiles[tile_index].shader_batches[triangle.shader_id].push_back(triangle);
            }
        }
    }
}

void Render3D::rasterize_triangle_in_tile(const ScreenTriangle &triangle, const int tri_index, Tile &tile)
{
    Box2f bounds {
        Vec2d {
            std::min({ triangle.v0.pos.x, triangle.v1.pos.x, triangle.v2.pos.x }),
            std::min({ triangle.v0.pos.y, triangle.v1.pos.y, triangle.v2.pos.y })
        }.round(),
        Vec2d {
            std::max({ triangle.v0.pos.x, triangle.v1.pos.x, triangle.v2.pos.x }),
            std::max({ triangle.v0.pos.y, triangle.v1.pos.y, triangle.v2.pos.y })
        }.round()
    };

    Box2f clip_bounds {
        tile.screen_pos,
        tile.screen_pos + Vec2i(TILE_SIZE)
    };

    bounds.min.x = std::max(bounds.min.x, clip_bounds.min.x);
    bounds.min.y = std::max(bounds.min.y, clip_bounds.min.y);
    bounds.max.x = std::min(bounds.max.x, clip_bounds.max.x);
    bounds.max.y = std::min(bounds.max.y, clip_bounds.max.y);

    auto is_top_left = [](const Vec2f& a, const Vec2f& b)
    {
        return (a.y < b.y) || (a.y == b.y && a.x > b.x);
    };

    const double a { triangle.v1.pos.y - triangle.v2.pos.y };
    const double b { triangle.v2.pos.x - triangle.v1.pos.x };
    const double c {
        triangle.v1.pos.x * triangle.v2.pos.y -
        triangle.v1.pos.y * triangle.v2.pos.x
    };

    const double d { triangle.v2.pos.y - triangle.v0.pos.y };
    const double e { triangle.v0.pos.x - triangle.v2.pos.x };
    const double f {
        triangle.v2.pos.x * triangle.v0.pos.y -
        triangle.v2.pos.y * triangle.v0.pos.x
    };

    const double g { triangle.v0.pos.y - triangle.v1.pos.y };
    const double h { triangle.v1.pos.x - triangle.v0.pos.x };
    const double i {
        triangle.v0.pos.x * triangle.v1.pos.y -
        triangle.v0.pos.y * triangle.v1.pos.x
    };

    const bool tl0 = is_top_left(triangle.v1.pos, triangle.v2.pos);
    const bool tl1 = is_top_left(triangle.v2.pos, triangle.v0.pos);
    const bool tl2 = is_top_left(triangle.v0.pos, triangle.v1.pos);

    const double area { a * (triangle.v0.pos.x) + b * (triangle.v0.pos.y) + c };
    const double inv_area { 1.0f / area };

    if (area == 0.0)
    {
        return;
    }

    Vec3d w_row {
        a * (bounds.min.x + 0.5) + b * (bounds.min.y + 0.5) + c,
        d * (bounds.min.x + 0.5) + e * (bounds.min.y + 0.5) + f,
        g * (bounds.min.x + 0.5) + h * (bounds.min.y + 0.5) + i
    };

    for (int y = std::floor(bounds.min.y); y < std::ceil(bounds.max.y); ++y)
    {
        Vec3d w { w_row };

        for (int x = std::floor(bounds.min.x); x < std::ceil(bounds.max.x); ++x)
        {
            if ((w.x > 0 || (w.x == 0 && tl0)) &&
                (w.y > 0 || (w.y == 0 && tl1)) &&
                (w.z > 0 || (w.z == 0 && tl2)))
            {
                const double depth {
                    (w.x * triangle.v0.z_over_w +
                    w.y * triangle.v1.z_over_w +
                    w.z * triangle.v2.z_over_w) * inv_area
                };

                const int buffer_index {
                    (y - tile.screen_pos.y) * TILE_SIZE + (x - tile.screen_pos.x)
                };

                if (depth < tile.depth_buffer[buffer_index])
                {
                    tile.triangle_index_buffer[buffer_index] = tri_index;
                    tile.depth_buffer[buffer_index] = depth;
                }
            }

            w.x += a;
            w.y += d;
            w.z += g;
        }

        w_row.x += b;
        w_row.y += e;
        w_row.z += h;
    }
}

void Render3D::render_tile(Tile &tile, const std::map<int, Shader3D> &shaders, const double t) const
{
    const Vec2i resolution { surface->get_resolution() };
    const int num_pixels { TILE_SIZE * TILE_SIZE };

    for (const auto &[shader_id, tri] : tile.shader_batches)
    {
        int triangle_index { 0 };
        for (const auto &tri : tri)
        {
            rasterize_triangle_in_tile(tri, triangle_index, tile);
            triangle_index++;
        }

        auto frag_shader { shaders.at(shader_id).get_fragment_shader() };
        frag_shader->set_uniforms(
            Shader3D::FragUniforms {
                .t = t,
                .light_dir = light_dir,
                .ambient_intensity = ambient_light,
                .near_plane = get_camera().get_z_near(),
                .far_plane = get_camera().get_z_far()
            }
        );

        for (int i = 0; i < num_pixels; ++i)
        {
            const Vec2i pixel_pos {
                tile.screen_pos.x + (i % TILE_SIZE),
                tile.screen_pos.y + (i / TILE_SIZE)
            };

            if (pixel_pos.x >= resolution.x ||
                pixel_pos.y >= resolution.y)
            {
                continue;
            }

            const double depth { tile.depth_buffer[i] };

            if (depth < std::numeric_limits<float>::infinity())
            {
                const ScreenTriangle &triangle { 
                    tri[tile.triangle_index_buffer[i]] 
                };

                const Vec3d w {
                    (triangle.v1.pos.y - triangle.v2.pos.y) * (pixel_pos.x - triangle.v2.pos.x) +
                    (triangle.v2.pos.x - triangle.v1.pos.x) * (pixel_pos.y - triangle.v2.pos.y),

                    (triangle.v2.pos.y - triangle.v0.pos.y) * (pixel_pos.x - triangle.v2.pos.x) +
                    (triangle.v0.pos.x - triangle.v2.pos.x) * (pixel_pos.y - triangle.v2.pos.y),

                    (triangle.v0.pos.y - triangle.v1.pos.y) * (pixel_pos.x - triangle.v1.pos.x) +
                    (triangle.v1.pos.x - triangle.v0.pos.x) * (pixel_pos.y - triangle.v1.pos.y)
                };

                const Vec3d normal_interp {
                    (triangle.v0.normal * w.x +
                    triangle.v1.normal * w.y +
                    triangle.v2.normal * w.z).normalize()
                };

                const Color4 color_interp {
                    triangle.v0.color == triangle.v1.color &&
                    triangle.v1.color == triangle.v2.color ?

                    triangle.v0.color :

                    Color4::trilinear_interp(
                        triangle.v0.color,
                        triangle.v1.color,
                        triangle.v2.color,
                        w.x,
                        w.y,
                        w.z
                    )
                };

                const Shader3D::FragInput frag_in {
                    .uvw = Vec3d { 0.0, 0.0, 0.0 },
                    .depth = depth,
                    .normal = normal_interp,
                    .color = color_interp
                };

                Color4 color { frag_shader->frag(frag_in) };
                surface->write_pixel(
                    pixel_pos,
                    color,
                    depth,
                    RenderSurface::BlendMode::ALPHA
                );
            }
        }
    }

    for (const auto &shader : fullscreen_shaders)
    {
        for (int i = 0; i < num_pixels; ++i)
        {
            const Vec2i pixel_pos {
                tile.screen_pos.x + (i % TILE_SIZE),
                tile.screen_pos.y + (i / TILE_SIZE)
            };

            Color4 color {
                shader->frag(
                    Shader3D::FragInput {
                        .uvw = Vec3d {
                            static_cast<double>(pixel_pos.x) / static_cast<double>(resolution.x),
                            static_cast<double>(pixel_pos.y) / static_cast<double>(resolution.y),
                            0.0
                        },
                        .depth = tile.depth_buffer[i],
                        .normal = Vec3d { 0.0, 0.0, 1.0 },
                        .color = surface->read_pixel(pixel_pos)
                    }
                )
            };

            surface->write_pixel(
                pixel_pos,
                color,
                tile.depth_buffer[i],
                RenderSurface::BlendMode::ALPHA
            );
        }
    }
}

void Render3D::clear_frame() const
{
    surface->clear_frame_buffer();
    surface->clear();
}

void Render3D::present_frame() const
{
    surface->present();
}

std::shared_ptr<SceneGraph3D> Render3D::get_scene_graph() const
{
    return scene_graph;
}

std::shared_ptr<RenderSurface> Render3D::get_render_surface() const
{
    return surface;
}

std::shared_ptr<Cuboid3D> Render3D::create_cuboid(
    const Vec3d position,
    const Vec3d size,
    const Color4 color,
    const Shader3D shader
)
{
    auto cuboid { std::make_shared<Cuboid3D>() };

    cuboid->set_position(position);
    cuboid->set_size(size);
    cuboid->set_color(color);
    cuboid->set_shader(shader);

    return cuboid;
}

std::shared_ptr<Plane3D> Render3D::create_plane(
    const Vec3d position,
    const Vec2d size,
    const Color4 color,
    const Shader3D shader
)
{
    auto plane { std::make_shared<Plane3D>() };

    plane->set_position(position);
    plane->set_size(size);
    plane->set_color(color);
    plane->set_shader(shader);

    return plane;
}

std::shared_ptr<Sphere3D> Render3D::create_sphere(
    const Vec3d position,
    const double radius,
    const Color4 color,
    const int segments,
    const Shader3D shader
)
{
    auto sphere { std::make_shared<Sphere3D>() };

    sphere->set_position(position);
    sphere->set_radius(radius);
    sphere->set_color(color);
    sphere->set_num_segments(segments);
    sphere->set_shader(shader);

    return sphere;
}

std::shared_ptr<Cone3D> Render3D::create_cone(
    const Vec3d position,
    const double radius,
    const double height,
    const Color4 color,
    const int segments,
    const Shader3D shader
)
{
    auto cone { std::make_shared<Cone3D>() };

    cone->set_position(position);
    cone->set_radius(radius);
    cone->set_height(height);
    cone->set_color(color);
    cone->set_segments(segments);
    cone->set_shader(shader);

    return cone;
}

void Render3D::add_item(const std::shared_ptr<Primitive3D> &item) const
{
    scene_graph->add_item(item);
}

void Render3D::remove_item(const std::shared_ptr<Primitive3D> &item) const
{
    scene_graph->remove_item(item);
}

bool Render3D::contains_item(const std::shared_ptr<Primitive3D> &item) const
{
    return scene_graph->contains_item(item);
}

void Render3D::clear_items() const
{
    scene_graph->clear();
}

int Render3D::num_items() const
{
    return scene_graph->num_items();
}

Vec2i Render3D::get_resolution() const
{
    return surface->get_resolution();
}

void Render3D::set_camera(const Camera &cam)
{
    camera = cam;
}

const Camera &Render3D::get_camera() const
{
    return camera;
}

Camera &Render3D::get_camera()
{
    return camera;
}

void Render3D::set_light_direction(const Vec3d dir)
{
    light_dir = dir.normalize();
}

Vec3d Render3D::get_light_direction() const
{
    return light_dir;
}

void Render3D::set_ambient_light(const double intensity)
{
    ambient_light = intensity;
}

double Render3D::get_ambient_light() const
{
    return ambient_light;
}

void Render3D::set_render_surface(const std::shared_ptr<RenderSurface> new_surface)
{
    surface = new_surface;
}

void Render3D::add_fullscreen_shader(const std::shared_ptr<Shader3D::FragShader> shader)
{
    fullscreen_shaders.push_back(shader);
}

void Render3D::remove_fullscreen_shader(const std::shared_ptr<Shader3D::FragShader> shader)
{
    std::erase(fullscreen_shaders, shader);
}

void Render3D::clear_fullscreen_shaders()
{
    fullscreen_shaders.clear();
}

}
