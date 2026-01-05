#include "gfx/core/render-3D.h"

#include "gfx/geometry/rasterize.h"

namespace gfx
{

void Render3D::draw_frame()
{
    Vec2i resolution { surface->get_resolution() };
    double t { std::chrono::duration<double, std::milli>(
        std::chrono::high_resolution_clock::now().time_since_epoch()
    ).count() / 1000.0 };

    for (const auto& [primitive, transform] : scene_graph->get_draw_queue())
    {
        PolygonMesh mesh { primitive->get_mesh() };

        std::shared_ptr<Shader3D> shader { primitive->get_shader() };
        shader->get_vertex_shader()->set_matrices(
            transform,
            camera.get_view_matrix(),
            camera.get_projection_matrix(get_aspect_ratio())
        );

        for (const auto& tri : mesh.get_triangles())
        {
            std::array<PolygonMesh::Vertex, 3> triangle_vertices {
                mesh.get_vertices().at(tri.v0),
                mesh.get_vertices().at(tri.v1),
                mesh.get_vertices().at(tri.v2)
            };

            std::array<Shader3D::VertOutput, 3> verts;

            for (size_t i = 0; i < 3; i++)
            {
                const auto& vertex { triangle_vertices[i] };
                Shader3D::VertInput vert_input { 
                    .pos = vertex.pos,
                    .normal = vertex.pos
                };
                verts[i] = primitive->get_shader()->vert(vert_input);
            }

            struct ScreenVertex
            {
                Vec2d screen_pos;
                double z_over_w;
                double inv_w;
            };

            std::array<ScreenVertex, 3> screen_vertices;
            bool skip_triangle { false };

            for (int i = 0; i < 3 && !skip_triangle; ++i)
            {
                if (verts[i].w <= 0.0)
                {
                    skip_triangle = true;
                    break;
                }

                const auto& clip { verts[i].xyz };

                double inv_w { 1.0 / verts[i].w };

                Vec3d ndc {
                    clip.x * inv_w,
                    clip.y * inv_w,
                    clip.z * inv_w
                };

                screen_vertices[i].screen_pos = {
                    (ndc.x * 0.5 + 0.5) * resolution.x,
                    (1.0 - (ndc.y * 0.5 + 0.5)) * resolution.y
                };

                screen_vertices[i].z_over_w = ndc.z;
                screen_vertices[i].inv_w = inv_w;
            }
            if (skip_triangle)
            {
                continue;
            }

            BarycentricTriangle triangle {
                screen_vertices[0].screen_pos,
                screen_vertices[1].screen_pos,
                screen_vertices[2].screen_pos
            };

            double z_over_w[3] = {
                screen_vertices[0].z_over_w,
                screen_vertices[1].z_over_w,
                screen_vertices[2].z_over_w
            };

            double one_over_w[3] = {
                screen_vertices[0].inv_w,
                screen_vertices[1].inv_w,
                screen_vertices[2].inv_w
            };

            Rasterize::rasterize_filled_triangle(triangle, primitive->get_color(), [&](const Pixel &pixel) {
                Vec3d w = triangle.barycentric_weights(pixel.position);

                // double inv_w_p = 
                //     w.x * one_over_w[0] +
                //     w.y * one_over_w[1] +
                //     w.z * one_over_w[2];

                double depth = 
                    w.x * z_over_w[0] +
                    w.y * z_over_w[1] +
                    w.z * z_over_w[2];

                double depth_normalized { depth };

                Shader3D::FragInput frag_input;
                frag_input.depth = depth_normalized;
                frag_input.t = t;

                Color4 color = primitive->get_shader()->frag(frag_input);

                surface->write_pixel(pixel.position, color, depth);
            });
        }
    }
}

}
