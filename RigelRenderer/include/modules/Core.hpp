#pragma once

#include "glm.hpp"

struct GLFWwindow;

namespace rgr
{
    class Scene;

    class Core
    {
    public:
        static bool Init(size_t width, size_t height, const char* title);
        static void Update();
        static void LoadScene(rgr::Scene* scene);
        static bool AppShouldRun();
        static void Quit();
        static glm::vec2 GetWindowSize();
    private:
        static GLFWwindow* m_Window;
        static Scene* m_LoadedScene;
        static size_t m_ScreenWidth;
        static size_t m_ScreenHeight;

        static void OnScreenResize();

        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
    };
}
