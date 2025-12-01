#include <iostream>
#include <algorithm>

#include "gfx/surfaces/glfw/glfw-render-surface.h"

namespace gfx
{

int GLFWRenderSurface::init()
{
    clear_color = Color4(0.2, 0.2, 0.2, 1.0);

    if (!glfwInit()) 
    {
        throw std::runtime_error("Failed to initialize GLFW");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_RED_BITS, 8);
    glfwWindowHint(GLFW_GREEN_BITS, 8);
    glfwWindowHint(GLFW_BLUE_BITS, 8);
    glfwWindowHint(GLFW_ALPHA_BITS, 8);

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    if (!monitor) 
    {
        glfwTerminate();
        throw std::runtime_error("Failed to get primary monitor");
        return -1;
    }

    GLFWvidmode const* mode = glfwGetVideoMode(monitor);
    if (!mode) 
    {
        glfwTerminate();
        throw std::runtime_error("Failed to get video mode");
        return -1;
    }

    gl_window_size = { mode->width, mode->height };
    refresh_rate_hz = mode->refreshRate;

    window = glfwCreateWindow(resolution.x, resolution.y, window_title.c_str(), nullptr, nullptr);
    if (!window) 
    {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
    {
        glfwDestroyWindow(window);
        glfwTerminate();
        throw std::runtime_error("Failed to initialize GLAD");
        return -1;
    }

    setup_texture();
    setup_quad();
    setup_shader();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glViewport(0, 0, resolution.x, resolution.y);
    
    glClearColor(
        clear_color.r_double(), 
        clear_color.g_double(), 
        clear_color.b_double(), 
        clear_color.a_double()
    );

    return 0;
}

void GLFWRenderSurface::present()
{
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexSubImage2D(
        GL_TEXTURE_2D, 0, 0, 0, resolution.x, resolution.y, 
        GL_RGBA, GL_UNSIGNED_BYTE, frame_buffer->data()
    );

    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shader_program);
    glBindVertexArray(vao);
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void GLFWRenderSurface::clear() const
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void GLFWRenderSurface::clear_frame_buffer()
{
    std::fill(frame_buffer->begin(), frame_buffer->end(), 0);
}

void GLFWRenderSurface::resize(const Vec2i new_resolution)
{
    resolution = new_resolution;
    frame_buffer->resize(resolution.x * resolution.y, 0);
    if (window)
    {
        glfwSetWindowSize(window, new_resolution.x, new_resolution.y);
    }
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA, resolution.x, resolution.y, 
        0, GL_RGBA, GL_UNSIGNED_BYTE, frame_buffer->data()
    );
    glViewport(0, 0, resolution.x, resolution.y);
}

void GLFWRenderSurface::print_frame_buffer()
{
    for (int y = 0; y < resolution.y; y += 32)
    {
        for (int x = 0; x < resolution.x; x += 32)
        {
            if (frame_buffer->at(y * resolution.x + x) != 0)
            {
                std::cout << "*";
            }
            else
            {
                std::cout << " ";
            }
        }
        std::cout << std::endl;
    }
}

void GLFWRenderSurface::setup_texture()
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, resolution.x, resolution.y,
                    0, GL_RGBA, GL_UNSIGNED_BYTE, frame_buffer->data());
}

void GLFWRenderSurface::setup_quad()
{
    float vertices[] = {
        -1.f, -1.f, 0.f, 1.f,
        1.f, -1.f, 1.f, 1.f,
        -1.f,  1.f, 0.f, 0.f,
        1.f,  1.f, 1.f, 0.f
    };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

void GLFWRenderSurface::setup_shader()
{
    const char* vert_src = R"(
    #version 330 core
    layout(location = 0) in vec2 aPos;
    layout(location = 1) in vec2 aTex;
    out vec2 TexCoord;
    void main() { TexCoord = aTex; gl_Position = vec4(aPos, 0.0, 1.0); }
    )";

    const char* frag_src = R"(
    #version 330 core
    in vec2 TexCoord;
    out vec4 FragColor;
    uniform sampler2D screenTexture;
    void main() { FragColor = texture(screenTexture, TexCoord); }
    )";

    GLuint vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert, 1, &vert_src, nullptr);
    glCompileShader(vert);

    GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag, 1, &frag_src, nullptr);
    glCompileShader(frag);

    shader_program = glCreateProgram();
    glAttachShader(shader_program, vert);
    glAttachShader(shader_program, frag);
    glLinkProgram(shader_program);

    glDeleteShader(vert);
    glDeleteShader(frag);
}


}
