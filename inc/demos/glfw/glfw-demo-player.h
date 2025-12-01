#pragma once

// #include <iostream>
#include <gfx/core/render-2D.h>
#include <gfx/surfaces/glfw/glfw-render-surface.h>

#include "demos/common/core/demo-player.h"

namespace demos
{

class GLFWDemoPlayer : public demos::DemoPlayer
{

public:

    GLFWDemoPlayer() : demos::DemoPlayer()
    {
        const gfx::Vec2d resolution { 320, 200 };
        const gfx::Vec2d viewport_scaling { 1, 1 };

        auto surface = std::make_shared<gfx::GLFWRenderSurface>(resolution);
        renderer = std::make_shared<gfx::Render2D>(surface, viewport_scaling);

        glfwSetWindowUserPointer(surface->get_window(), this);

        glfwSetInputMode(surface->get_window(), GLFW_STICKY_KEYS, GLFW_TRUE);

        glfwSetMouseButtonCallback(surface->get_window(), mouse_callback);
        glfwSetScrollCallback(surface->get_window(), scroll_callback);
        glfwSetCursorPosCallback(surface->get_window(), cursor_callback);
        glfwSetCharCallback(surface->get_window(), char_callback);
    }

    gfx::Vec2i get_screen_size() override
    {
        return renderer->get_render_surface()->get_resolution();
        // int w, h;
        // glfwGetFramebufferSize(get_window(), &w, &h);
        // return { w, h };
    }

    int get_input() override
    {
        glfwPollEvents();
        return 0;
    }

    void draw_info() override
    {
        // auto surface = std::dynamic_pointer_cast<gfx::GLFWRenderSurface>
        //     (renderer->get_render_surface());
        // gfx::Vec2i resolution = renderer->get_render_surface()->get_resolution();
        // std::string fps = std::to_string(static_cast<int>(demos[current_demo]->get_fps()));
        // while (fps.size() < 4)
        // {
        //     fps = " " + fps;
        // }
        // int x, y;
        // glfwGetWindowSize(surface->get_window(), &x, &y);
        // surface->set_window_title(
        //     "Demo [" + std::to_string(current_demo + 1) + "/" + std::to_string(demos.size()) + "]" +
        //     " :: " + std::to_string(resolution.x) + "x" + std::to_string(resolution.y) +
        //     " :: " + fps + " FPS" +
        //     " :: " + std::to_string(renderer->num_items()) + " items" +
        //     "   [" + std::to_string(x) + "x" + std::to_string(y) + "]"
        //
        // );
        //
        // std::cout << "\033[2J\033[1;1H";
        // std::vector<std::string> debug_lines = demos[current_demo]->debug_text();
        // for (size_t i = 0; i < debug_lines.size(); ++i)
        // {
        //     std::cout << debug_lines[i] << std::endl;
        // }
    }

private:

    static void mouse_callback(GLFWwindow* win, int button, int action, int mods)
    {
        auto* self = static_cast<GLFWDemoPlayer*>(glfwGetWindowUserPointer(win));
        if (!self) 
        {
            return;
        }

        demos::MouseEvent event;
        if (button == GLFW_MOUSE_BUTTON_LEFT)
        {
            event.type = (action == GLFW_PRESS)
                ? demos::MouseEventType::LEFT_DOWN
                : demos::MouseEventType::LEFT_UP;
        }
        else if (button == GLFW_MOUSE_BUTTON_RIGHT)
        {
            event.type = (action == GLFW_PRESS)
                ? demos::MouseEventType::RIGHT_DOWN
                : demos::MouseEventType::RIGHT_UP;
        }

        double x, y;
        glfwGetCursorPos(win, &x, &y);

        int width, height;
        glfwGetWindowSize(win, &width, &height);

        gfx::Vec2d pos { 
            gfx::Vec2d { x, y } / 
            gfx::Vec2d { static_cast<double>(width), static_cast<double>(height) } 
        };
        event.position = pos;

        self->demos[self->current_demo]->report_mouse(event);
    }

    static void scroll_callback(GLFWwindow* win, double x_offset, double y_offset)
    {
        auto* self = static_cast<GLFWDemoPlayer*>(glfwGetWindowUserPointer(win));
        if (!self) return;

        demos::MouseEvent event;
        event.type = (y_offset > 0)
            ? demos::MouseEventType::SCROLL_UP
            : demos::MouseEventType::SCROLL_DOWN;

        double x, y;
        glfwGetCursorPos(win, &x, &y);
        event.position = gfx::Vec2i{ (int)x, (int)y } / self->renderer->get_viewport_scaling();

        self->demos[self->current_demo]->report_mouse(event);
    }

    static void cursor_callback(GLFWwindow* win, double xpos, double ypos)
    {
        auto* self = static_cast<GLFWDemoPlayer*>(glfwGetWindowUserPointer(win));
        if (!self) return;

        demos::MouseEvent event;
        event.type = demos::MouseEventType::MOVE;

        int width, height;
        glfwGetWindowSize(win, &width, &height);
        gfx::Vec2i window_size { width, height };

        // gfx::Vec2d position { gfx::Vec2d { xpos, ypos } / self->renderer->get_viewport_scaling() };
        gfx::Vec2d position { gfx::Vec2d { xpos, ypos } / window_size };
        // gfx::Vec2d pos {
        //     2.0 * (position.x / static_cast<double>(width)) - 1.0,
        //     1.0 - 2.0 * (position.y / static_cast<double>(height))
        // };
        event.position = position;

        self->demos[self->current_demo]->report_mouse(event);
    }

    static void char_callback(GLFWwindow* win, unsigned int key)
    {
        auto* self = static_cast<GLFWDemoPlayer*>(glfwGetWindowUserPointer(win));
        if (!self) 
        {
            return;
        }
        self->handle_input(key);
        self->demos[self->current_demo]->handle_input(key);
    }

    GLFWwindow* get_window()
    {
        return std::static_pointer_cast<gfx::GLFWRenderSurface>(renderer->get_render_surface())->get_window();
    }

    gfx::Vec2d get_scaling(GLFWwindow* win)
    {
        auto* self = static_cast<GLFWDemoPlayer*>(glfwGetWindowUserPointer(win));
        int width, height;
        glfwGetWindowSize(self->get_window(), &width, &height);

        int fb_width, fb_height;
        glfwGetFramebufferSize(self->get_window(), &fb_width, &fb_height);
        return gfx::Vec2d {
            static_cast<double>(fb_width) / static_cast<double>(width),
            static_cast<double>(fb_height) / static_cast<double>(height)
        };
    }
};

}
