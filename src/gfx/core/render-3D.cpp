#include "gfx/core/render-3D.h"

#include "gfx/geometry/rasterize.h"

#include <chrono>

namespace gfx
{

void Render3D::draw_frame()
{
    Vec2i resolution { surface->get_resolution() };

    double aspect_ratio { 
        static_cast<double>(resolution.x) / 
        static_cast<double>(resolution.y) 
    };

    double t { std::chrono::duration<double, std::milli>(
        std::chrono::high_resolution_clock::now().time_since_epoch()
    ).count() / 1000.0 };

    const Camera& camera { get_camera() };
    Matrix4x4d view_matrix { camera.get_view_matrix() };
    Matrix4x4d projection_matrix { camera.get_projection_matrix(aspect_ratio) };

    for (const auto& [primitive, transform] : scene_graph->get_draw_queue())
    {
        PolygonMesh mesh { primitive->get_mesh() };
        size_t num_vertices { mesh.get_vertices().size() };

        Shader3D shader { primitive->get_shader() };

        Shader3D::VertInput vert_in {
            .positions = mesh.get_vertices(),
            .normals = mesh.get_normals(),
            .model_matrix = transform,
            .view_matrix = view_matrix,
            .projection_matrix = projection_matrix,
            .mvp_matrix = projection_matrix * view_matrix * transform
        };

        Shader3D::VertOutput vert_out { shader.vert(vert_in) };

        struct ScreenVertex
        {
            Vec2d screen_pos;
            double z_over_w;
            double inv_w;
        };

        for (size_t i = 0; i < mesh.get_indices().size(); i += 3)
        {
            size_t idx0 { mesh.get_indices()[i] };
            size_t idx1 { mesh.get_indices()[i + 1] };
            size_t idx2 { mesh.get_indices()[i + 2] };

            if (vert_out.w[idx0] <= 0.0 ||
                vert_out.w[idx1] <= 0.0 ||
                vert_out.w[idx2] <= 0.0)
            {
                continue;
            }

            std::array<ScreenVertex, 3> screen_vertices;
            auto clip_to_screen { [&](const int source_index, const int dest_index) {
                const auto& clip { vert_out.xyz[source_index] };

                double inv_w { 1.0 / vert_out.w[source_index] };

                Vec3d ndc {
                    clip.x * inv_w,
                    clip.y * inv_w,
                    clip.z * inv_w
                };

                screen_vertices[dest_index].screen_pos = {
                    (ndc.x * 0.5 + 0.5) * resolution.x,
                    (1.0 - (ndc.y * 0.5 + 0.5)) * resolution.y
                };

                screen_vertices[dest_index].z_over_w = ndc.z;
                screen_vertices[dest_index].inv_w = inv_w;
            }};

            clip_to_screen(idx0, 0);
            clip_to_screen(idx1, 1);
            clip_to_screen(idx2, 2);

            BarycentricTriangle triangle {
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

            for (const auto& pixel : pixels)
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

                Vec3d normal_interp {(
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
                        w.x, w.y, w.z
                    )
                };

                frag_in.uvw.emplace_back(0.0, 0.0, 0.0);
                frag_in.depths.push_back(depth);
                frag_in.normals.push_back(normal_interp);
                frag_in.colors.push_back(color_interp);
            }

            auto pixel_colors { shader.frag(frag_in) };
            for (size_t i = 0; i < pixel_colors.size(); ++i)
            {
                surface->write_pixel(pixels[i], pixel_colors[i], frag_in.depths[i]);
            }
        }
    }
}

Render3D::Render3D(std::shared_ptr<RenderSurface> surface) :
    surface(surface),
    scene_graph(std::make_shared<SceneGraph3D>())
{
}

void Render3D::clear_frame()
{
    surface->clear_frame_buffer();
    surface->clear();
}

void Render3D::present_frame()
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

std::shared_ptr<Cuboid3D> Render3D::create_cuboid(const Vec3d position, const Vec3d size, const Color4 color, const Shader3D shader) const
{
    auto cuboid { std::make_shared<Cuboid3D>() };

    cuboid->set_position(position);
    cuboid->set_size(size);
    cuboid->set_color(color);
    cuboid->set_shader(shader);

    return cuboid;
}

std::shared_ptr<Plane3D> Render3D::create_plane(const Vec3d position, const Vec2d size, const Color4 color, const Shader3D shader) const
{
    auto plane { std::make_shared<Plane3D>() };

    plane->set_position(position);
    plane->set_size(size);
    plane->set_color(color);
    plane->set_shader(shader);

    return plane;
}

std::shared_ptr<Sphere3D> Render3D::create_sphere(const Vec3d position, const double radius, const Color4 color, const int segments, const Shader3D shader) const
{
    auto sphere { std::make_shared<Sphere3D>() };

    sphere->set_position(position);
    sphere->set_radius(radius);
    sphere->set_color(color);
    sphere->set_num_segments(segments);
    sphere->set_shader(shader);

    return sphere;
}

void Render3D::add_item(const std::shared_ptr<Primitive3D>& item)
{
    scene_graph->add_item(item);
}

void Render3D::remove_item(const std::shared_ptr<Primitive3D>& item)
{
    scene_graph->remove_item(item);
}

void Render3D::clear_items()
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

void Render3D::set_camera(const Camera& cam)
{
    camera = cam;
}

const Camera& Render3D::get_camera() const
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
