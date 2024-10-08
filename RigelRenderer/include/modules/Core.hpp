#pragma once

#include "glm.hpp"
#include "Internal.hpp"

#include <memory>
#include <string>

struct GLFWwindow;

namespace rgr
{
    class Scene;
    class Renderer;

    class Core
    {
    public:
        static bool Init(size_t width, size_t height, const char* title, const bool vsync = true);
        static void Update();
        static void LoadScene(rgr::Scene* scene);
        static bool AppShouldRun();
        static void Quit();
        static glm::vec2 GetWindowSize();

        static void EnableDebugGUI();
        static void DisableDebugGUI();

        static void EnableVSync();
        static void DisableVSync();

        static void SetTargetFPS(const size_t fps);

        static void SetBuiltInResourcesPath(const std::string& dir);
        static std::string& GetBuiltInResourcesPath();
    private:
        static GLFWwindow* m_Window;
        static Scene* m_LoadedScene;
        static size_t m_ScreenWidth;
        static size_t m_ScreenHeight;
        static Renderer* m_Renderer;
        static bool m_DrawDebugGUI;
        static bool m_UseVSync;
        static double m_TargetFrameTime;
        static std::string m_BuiltInResourcesPath;

        static void OnScreenResize();
        static void ProcessInput();
        static void DrawDebugGUI();

        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
        static void __stdcall gl_debug_output_callback(uint32_t source, uint32_t type, uint32_t id, uint32_t severity,
                                                       int length, const char *message, const void *userParam);
    INTERNAL:
        inline static GLFWwindow* GetWindowPtr() { return m_Window; }
        inline static Renderer* GetRendererInstance() { return m_Renderer; }
    };
}
