#include "demos/common/animations/test-3D/test-3D-demo.h"

#include "demos/common/core/demo-utils.h"
#include "gfx/core/camera.h"

using namespace gfx;

namespace demos
{

void Test3DDemo::init()
{
    Camera camera;
    camera.set_position({ 0.0, 0.0, -5.0 });
    camera.set_rotation({ 0.0, 0.0, 0.0 });
    camera.set_z_near(0.1);
    camera.set_z_far(1000.0);
    render3D->set_camera(camera);

    std::vector<PolygonMesh::Vertex> vertices = {
        { { -1.0, -1.0, 0.0 } },
        { {  1.0, -1.0, 0.0 } },
        { {  0.0,  1.0, 0.0 } }
    };

    std::vector<PolygonMesh::Triangle> triangles = {
        { 0, 1, 2 }
    };

    std::vector<PolygonMesh::Vertex> box_vertices = {
        { { -1.0, -1.0, -1.0 } },
        { {  1.0, -1.0, -1.0 } },
        { {  1.0,  1.0, -1.0 } },
        { { -1.0,  1.0, -1.0 } },
        { { -1.0, -1.0,  1.0 } },
        { {  1.0, -1.0,  1.0 } },
        { {  1.0,  1.0,  1.0 } },
        { { -1.0,  1.0,  1.0 } }
    };

    std::vector<PolygonMesh::Triangle> box_triangles = {
        { 0, 1, 2 }, { 0, 2, 3 },
        { 4, 5, 6 }, { 4, 6, 7 },
        { 0, 1, 5 }, { 0, 5, 4 },
        { 2, 3, 7 }, { 2, 7, 6 },
        { 1, 2, 6 }, { 1, 6, 5 },
        { 0, 3, 7 }, { 0, 7, 4 }
    };

    PolygonMesh mesh(box_vertices, box_triangles);

    item = std::make_shared<Primitive3D>();
    item->mesh = mesh;

    render3D->add_item(item);
}

void Test3DDemo::render_frame(const double dt)
{
    double t0 { time_sec() };

    item->set_rotation_degrees(
        item->get_rotation_degrees().x + dt * 30.0,
        item->get_rotation_degrees().y + dt * 45.0,
        item->get_rotation_degrees().z + dt * 60.0
    );

    item->set_position(
        item->get_position().x,
        std::sin(t0) * 2.0,
        std::cos(t0) * 2.0
    );

    item->set_scale(
        1.0,
        1.0,
        1.0
    );

    render2D->clear_frame();
    render3D->draw_frame();
    render2D->draw_frame();
    render2D->present_frame();
}

void Test3DDemo::handle_input(const int input)
{
    Vec3d pos { render3D->get_camera().get_position() };
    Vec3d rot { render3D->get_camera().get_rotation() };
    switch (input)
    {
        case 'w':
            {
                render3D->set_camera_position({
                    pos.x + std::sin(rot.y * (std::numbers::pi / 180.0)) * 0.5,
                    pos.y,
                    pos.z + std::cos(rot.y * (std::numbers::pi / 180.0)) * 0.5
                });
                break;
            }
        case 's':
            {
                render3D->set_camera_position({
                    pos.x - std::sin(rot.y * (std::numbers::pi / 180.0)) * 0.5,
                    pos.y,
                    pos.z - std::cos(rot.y * (std::numbers::pi / 180.0)) * 0.5
                });
                break;
            }
    }
}

void Test3DDemo::report_mouse(const demos::MouseEvent event)
{
    switch (event.type)
    {
        case MouseEventType::MOVE:
            {
                Vec2d delta { event.position - prev_mouse_pos };
                render3D->set_camera_rotation({
                    render3D->get_camera().get_rotation().x + delta.y,
                    render3D->get_camera().get_rotation().y + delta.x,
                    render3D->get_camera().get_rotation().z
                });
                prev_mouse_pos = event.position;
                break;
            }
        case MouseEventType::LEFT_DOWN:
            {
                break;
            }
        case MouseEventType::LEFT_UP:
            {
                break;
            }
        case MouseEventType::RIGHT_DOWN:
            {
                break;
            }
        case MouseEventType::RIGHT_UP:
            {
                break;
            }
        case MouseEventType::SCROLL_UP:
            {
                break;
            }
        case MouseEventType::SCROLL_DOWN:
            {
                break;
            }
    }
}

void Test3DDemo::end()
{
    render2D->clear_items();
    render3D->clear_items();
}

}
