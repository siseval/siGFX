#pragma once

#include <string>

#include "gfx/core/render-surface.h"
#include "gfx/surfaces/glfw/glad.h"

#include <GLFW/glfw3.h>

namespace gfx
{
    class GLFWRenderSurface final : public RenderSurface
    {

    public:

        explicit GLFWRenderSurface(const Vec2i resolution)
            : RenderSurface(resolution)
        {
            init();
        }

        int init() override;

        void present() override;
        void clear() const override;

        void clear_palette() override {};
        void clear_frame_buffer() override;

        void resize(Vec2i new_resolution) override;

        void print_frame_buffer() const;

        GLFWwindow* get_window() const
        {
            return window;
        }

        void set_window_title(const std::string title)
        {
            window_title = title;
            if (window)
            {
                glfwSetWindowTitle(window, window_title.c_str());
            }
        }

        void set_clear_color(const Color4 color) override
        {
            clear_color = color;
            glClearColor(
                clear_color.r_double(),
                clear_color.g_double(),
                clear_color.b_double(),
                clear_color.a_double()
            );
        }

    private:

        void render_multithreaded();

        void setup_texture();
        void setup_quad();
        void setup_shader();

        GLFWwindow* window;

        Vec2i gl_window_size{800, 600};
        double refresh_rate_hz = 60.0;

        GLuint texture = 0;
        GLuint vao = 0;
        GLuint vbo = 0;
        GLuint shader_program = 0;

        std::string window_title = "GLFW Render Surface";
    };
}
