#include "gfx/core/render-3D.h"

#include "gfx/math/box2.h"

#include <chrono>
#include <map>
#include <cmath>
#include <algorithm>

namespace gfx
{

Render3D::Render3D(std::shared_ptr<RenderSurface> surface)
    : ambient_light(0.0),
      scene_graph(std::make_shared<SceneGraph3D>()),
      surface(surface) {}


void Render3D::draw_frame() const
{
    Vec2i resolution { surface->get_resolution() };

    double t {
        std::chrono::duration<double, std::milli>(
            std::chrono::high_resolution_clock::now().time_since_epoch()
        ).count() / 1000.0
    };

    std::map<int, Shader3D> shader_map;

    std::vector<ScreenTriangle> screen_triangles {
        generate_screen_triangles(shader_map)
    };

    std::vector<std::vector<Tile>> tiles {
        generate_tiles()
    };

    bin_triangles(screen_triangles, tiles);

    Vec2i num_tiles {
        static_cast<int>(std::ceil(static_cast<float>(resolution.x) / TILE_SIZE)),
        static_cast<int>(std::ceil(static_cast<float>(resolution.y) / TILE_SIZE))
    };

    for (int y = 0; y < num_tiles.y; y++)
    {
        for (int x = 0; x < num_tiles.x; x++)
        {
            if (tiles[y][x].shader_batches.empty())
            {
                continue;
            }
            render_tile(tiles[y][x], shader_map, t);
        }
    }
}

std::vector<Render3D::ScreenTriangle> Render3D::generate_screen_triangles(std::map<int, Shader3D> &shader_map) const
{
    std::vector<ScreenTriangle> screen_triangles;

    Vec2i resolution { surface->get_resolution() };

    double aspect_ratio {
        static_cast<double>(resolution.x) /
        static_cast<double>(resolution.y)
    };

    const Camera &camera { get_camera() };
    Matrix4x4d view_matrix { camera.get_view_matrix() };
    Matrix4x4d projection_matrix { camera.get_projection_matrix(aspect_ratio) };

    for (const auto &[primitive, transform] : scene_graph->get_draw_queue())
    {
        PolygonMesh mesh { primitive->get_mesh() };

        Shader3D shader { primitive->get_shader() };

        shader.get_vertex_shader()->set_uniforms(
            Shader3D::VertUniforms {
                .model_matrix = primitive->get_transform(),
                .view_matrix = view_matrix,
                .projection_matrix = projection_matrix,
                .mvp_matrix = projection_matrix * view_matrix * primitive->get_transform()
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

                    screen_vertices[dest_index].screen_pos = {
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
                    screen_vertices[1].screen_pos - screen_vertices[0].screen_pos,
                    screen_vertices[2].screen_pos - screen_vertices[0].screen_pos
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

std::vector<std::vector<Render3D::Tile>> Render3D::generate_tiles() const
{
    Vec2i resolution { surface->get_resolution() };

    const int num_tiles_x {
        static_cast<int>(std::ceil(static_cast<float>(resolution.x) / TILE_SIZE))
    };
    const int num_tiles_y {
        static_cast<int>(std::ceil(static_cast<float>(resolution.y) / TILE_SIZE))
    };

    std::vector<std::vector<Tile>> tiles;
    tiles.reserve(num_tiles_y);

    for (int ty = 0; ty < num_tiles_y; ty++)
    {
        tiles.emplace_back();
        tiles[ty].reserve(num_tiles_x);

        for (int tx = 0; tx < num_tiles_x; tx++)
        {
            tiles[ty].emplace_back(Vec2i { tx * TILE_SIZE, ty * TILE_SIZE });
        }
    }

    return tiles;
}

void Render3D::bin_triangles(const std::vector<ScreenTriangle> &screen_triangles, std::vector<std::vector<Tile>> &tiles) const
{
    Vec2i resolution { surface->get_resolution() };

    Vec2i num_tiles { resolution.x / TILE_SIZE, resolution.y / TILE_SIZE };
    for (const auto &triangle : screen_triangles)
    {
        struct Edge
        {
            double a;
            double b;
            double c;

            explicit Edge(const Vec2d p0, const Vec2d p1)
                : a(p0.y - p1.y),
                  b(p1.x - p0.x),
                  c(p0.x * p1.y - p0.y * p1.x) {}

            double eval(const Vec2d p) const
            {
                return a * p.x + b * p.y + c;
            }
        };

        const Vec2d tv0 { triangle.v0.screen_pos / TILE_SIZE };
        const Vec2d tv1 { triangle.v1.screen_pos / TILE_SIZE };
        const Vec2d tv2 { triangle.v2.screen_pos / TILE_SIZE };

        // const Edge e0(tv1, tv2);
        // const Edge e1(tv2, tv0);
        // const Edge e2(tv0, tv1);
        // const std::array<Edge, 3> edges { e0, e1, e2 };

        const Box2d tri_bounds {
            Vec2d {
                std::floor(std::min({ tv0.x, tv1.x, tv2.x })),
                std::floor(std::min({ tv0.y, tv1.y, tv2.y }))
            },
            Vec2d {
                std::ceil(std::max({ tv0.x, tv1.x, tv2.x })),
                std::ceil(std::max({ tv0.y, tv1.y, tv2.y }))
            }
        };

        // auto overlaps_tile {
        //     [&](const Vec2d tile) {
        //         const std::array<Vec2d, 4> corners {
        //             Vec2d { tile.x, tile.y },
        //             Vec2d { tile.x + 1, tile.y },
        //             Vec2d { tile.x, tile.y + 1 },
        //             Vec2d { tile.x + 1, tile.y + 1 }
        //         };
        //
        //         for (auto edge : edges)
        //         {
        //             if (edge.eval(corners[0]) > 0 &&
        //                 edge.eval(corners[1]) > 0 &&
        //                 edge.eval(corners[2]) > 0 &&
        //                 edge.eval(corners[3]) > 0)
        //             {
        //                 return true;
        //             }
        //         }
        //         return true;
        //     }
        // };

        for (int y = tri_bounds.min.y; y < tri_bounds.max.y; y++)
        {
            if (y < 0 || y >= num_tiles.y)
            {
                continue;
            }
            for (int x = tri_bounds.min.x; x < tri_bounds.max.x; x++)
            {
                if (x < 0 || x >= num_tiles.x)
                {
                    continue;
                }
                if (true) //overlaps_tile(Vec2d { static_cast<double>(x), static_cast<double>(y) }))
                {
                    tiles[y][x].shader_batches[triangle.shader_id].push_back(triangle);
                }
            }
        }
    }
}



void Render3D::rasterize_triangle_in_tile(const ScreenTriangle &triangle, const int tri_index, Tile &tile)
{
    Box2f bounds {
        Vec2d {
            std::min({ triangle.v0.screen_pos.x, triangle.v1.screen_pos.x, triangle.v2.screen_pos.x }),
            std::min({ triangle.v0.screen_pos.y, triangle.v1.screen_pos.y, triangle.v2.screen_pos.y })
        }.round(),
        Vec2d {
            std::max({ triangle.v0.screen_pos.x, triangle.v1.screen_pos.x, triangle.v2.screen_pos.x }),
            std::max({ triangle.v0.screen_pos.y, triangle.v1.screen_pos.y, triangle.v2.screen_pos.y })
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

    // Box2f bounds { Box2f::unexpanded() };
    // bounds.expand(
    //     Vec2f::clamp(
    //         triangle.v0.screen_pos,
    //         tile.screen_pos,
    //         tile.screen_pos + Vec2i(TILE_SIZE)
    //     )
    // );
    // bounds.expand(
    //     Vec2f::clamp(
    //         triangle.v1.screen_pos,
    //         tile.screen_pos,
    //         tile.screen_pos + Vec2i(TILE_SIZE)
    //     )
    // );
    // bounds.expand(
    //     Vec2f::clamp(
    //         triangle.v2.screen_pos,
    //         tile.screen_pos,
    //         tile.screen_pos + Vec2i(TILE_SIZE)
    //     )
    // );
    //
    auto is_top_left = [](const Vec2f& a, const Vec2f& b)
    {
        return (a.y < b.y) || (a.y == b.y && a.x > b.x);
    };

    const double a { triangle.v1.screen_pos.y - triangle.v2.screen_pos.y };
    const double b { triangle.v2.screen_pos.x - triangle.v1.screen_pos.x };
    const double c {
        triangle.v1.screen_pos.x * triangle.v2.screen_pos.y -
        triangle.v1.screen_pos.y * triangle.v2.screen_pos.x
    };

    const double d { triangle.v2.screen_pos.y - triangle.v0.screen_pos.y };
    const double e { triangle.v0.screen_pos.x - triangle.v2.screen_pos.x };
    const double f {
        triangle.v2.screen_pos.x * triangle.v0.screen_pos.y -
        triangle.v2.screen_pos.y * triangle.v0.screen_pos.x
    };

    const double g { triangle.v0.screen_pos.y - triangle.v1.screen_pos.y };
    const double h { triangle.v1.screen_pos.x - triangle.v0.screen_pos.x };
    const double i {
        triangle.v0.screen_pos.x * triangle.v1.screen_pos.y -
        triangle.v0.screen_pos.y * triangle.v1.screen_pos.x
    };

    const bool tl0 = is_top_left(triangle.v1.screen_pos, triangle.v2.screen_pos);
    const bool tl1 = is_top_left(triangle.v2.screen_pos, triangle.v0.screen_pos);
    const bool tl2 = is_top_left(triangle.v0.screen_pos, triangle.v1.screen_pos);

    const double area { a * (triangle.v0.screen_pos.x) + b * (triangle.v0.screen_pos.y) + c };
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
                    tile.weight_buffer[buffer_index] = Vec2d { w.x, w.y } * inv_area;
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
    Vec2i resolution { surface->get_resolution() };

    for (const auto &[shader_id, triangles] : tile.shader_batches)
    {
        int triangle_index { 0 };
        for (const auto &tri : triangles)
        {
            rasterize_triangle_in_tile(tri, triangle_index, tile);
            triangle_index++;
        }

        auto frag_shader { shaders.at(shader_id).get_fragment_shader() };
        frag_shader->set_uniforms(
            Shader3D::FragUniforms {
                .t = t,
                .light_dir = light_dir,
                .ambient_intensity = ambient_light
            }
        );

        for (int i = 0; i < TILE_SIZE * TILE_SIZE; ++i)
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
                Vec2f w_xy { tile.weight_buffer[i] };
                Vec3f w { w_xy.x, w_xy.y, 1.0f - w_xy.x - w_xy.y };

                const ScreenTriangle &triangle { 
                    triangles[tile.triangle_index_buffer[i]] 
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
                    RenderSurface::BlendMode::OPAQUE
                );
            }
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

double Render3D::get_camera_aspect_ratio() const
{
    return static_cast<double>(get_resolution().x) / static_cast<double>(get_resolution().y);
}

void Render3D::set_camera(const Camera &cam)
{
    camera = cam;
}

const Camera &Render3D::get_camera() const
{
    return camera;
}

void Render3D::set_camera_position(const Vec3d pos)
{
    camera.set_position(pos);
}

Vec3d Render3D::get_camera_position() const
{
    return camera.get_position();
}

void Render3D::set_camera_rotation(const Vec3d rot)
{
    camera.set_rotation(rot);
}

void Render3D::set_camera_rotation_degrees(const Vec3d rot_deg)
{
    camera.set_rotation_degrees(rot_deg);
}

Vec3d Render3D::get_camera_rotation() const
{
    return camera.get_rotation();
}

void Render3D::set_camera_fov(const double fov)
{
    camera.set_fov(fov);
}

void Render3D::set_camera_fov_degrees(const double fov_degrees)
{
    camera.set_fov_degrees(fov_degrees);
}

void Render3D::set_camera_z_near(const double z_near)
{
    camera.set_z_near(z_near);
}

void Render3D::set_camera_z_far(const double z_far)
{
    camera.set_z_far(z_far);
}

double Render3D::get_camera_fov() const
{
    return camera.get_fov();
}

Vec3d Render3D::get_camera_forward() const
{
    return camera.get_forward();
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

}

/*BarycentricTriangle triangle {
                screen_vertices[0].screen_pos,
                screen_vertices[1].screen_pos,
                screen_vertices[2].screen_pos
            };

            if (triangle.get_area() <= 0.0)
            {
                continue;
            }

            std::vector<Vec2i> pixels;
            Rasterize::rasterize_filled_triangle(triangle, pixels, resolution);

            Shader3D::FragInput frag_in {
                .t = t,
                .light_dir = light_dir,
                .ambient_intensity = ambient_light
            };

            frag_in.uvw.reserve(pixels.size());
            frag_in.depths.reserve(pixels.size());
            frag_in.normals.reserve(pixels.size());
            frag_in.colors.reserve(pixels.size());

            auto vertex_colors { mesh.get_colors() };

            for (const auto &pixel : pixels)
            {
                Vec3d w = triangle.barycentric_weights(pixel + Vec2d { 0.5, 0.5 });

                // double inv_w_p =
                //     w.x * one_over_w[0] +
                //     w.y * one_over_w[1] +
                //     w.z * one_over_w[2];

                double depth {
                    w.x * screen_vertices[0].z_over_w +
                    w.y * screen_vertices[1].z_over_w +
                    w.z * screen_vertices[2].z_over_w
                };

                Vec3d normal_interp {
                    (
                        vert_out.normals[idx0] * w.x +
                        vert_out.normals[idx1] * w.y +
                        vert_out.normals[idx2] * w.z).normalize()
                };

                Color4 color_interp {
                    vertex_colors[idx0] == vertex_colors[idx1] &&
                    vertex_colors[idx1] == vertex_colors[idx2] ?

                        vertex_colors[idx0] :

                        Color4::trilinear_interp(
                            vertex_colors[idx0],
                            vertex_colors[idx1],
                            vertex_colors[idx2],
                            w.x,
                            w.y,
                            w.z
                        )
                };

                frag_in.uvw.emplace_back(0.0, 0.0, 0.0);
                frag_in.depths.push_back(depth);
                frag_in.normals.push_back(normal_interp);
                frag_in.colors.push_back(color_interp);
            }

            auto pixel_colors { shader.frag(frag_in) };
            for (size_t p = 0; p < pixel_colors.size(); ++p)
            {
                surface->write_pixel(
                    pixels[p],
                    pixel_colors[p],
                    frag_in.depths[p],
                    RenderSurface::BlendMode::OPAQUE
                );
            }
        }*/
