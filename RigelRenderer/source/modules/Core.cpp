#include "Core.hpp"
#include "Input.hpp"
#include "Time.hpp"
#include "Scene.hpp"
#include "render/RenderHandler.hpp"
#include "glad.h"
#include "glfw3.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>
#include <thread>

namespace rgr
{
    GLFWwindow* Core::m_Window = nullptr;
    rgr::Scene* Core::m_LoadedScene = nullptr;
    size_t Core::m_ScreenWidth = 0;
    size_t Core::m_ScreenHeight = 0;
    std::unique_ptr<RenderHandler> Core::m_RenderHandler = nullptr;
    bool Core::m_DrawDebugGUI = true;

    bool Core::Init(size_t width, size_t height, const char* title)
    {
        if (!glfwInit())
            return false;

        #ifdef _DEBUG
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
        #endif

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

        m_Window = glfwCreateWindow(static_cast<int>(width), static_cast<int>(height), title, nullptr, nullptr);
        if (!m_Window)
        {
            glfwTerminate();
            return false;
        }

        m_ScreenWidth = width;
        m_ScreenHeight = height;

        glfwMakeContextCurrent(m_Window);

        glfwSetKeyCallback(m_Window, key_callback);
        glfwSetFramebufferSizeCallback(m_Window, framebuffer_size_callback);
        glfwSetCursorPosCallback(m_Window, cursor_position_callback);

        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
        ImGui_ImplOpenGL3_Init();

        #ifdef _DEBUG
            int flags;
            glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
            if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
            {
                glEnable(GL_DEBUG_OUTPUT);
                glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
                glDebugMessageCallback(gl_debug_output_callback, nullptr);
                glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
            }
        #endif

        glViewport(0, 0, static_cast<int>(m_ScreenWidth), static_cast<int>(m_ScreenHeight));
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        m_RenderHandler = std::make_unique<rgr::RenderHandler>();

		glfwSetWindowAttrib(m_Window, GLFW_RESIZABLE, GLFW_FALSE);
//		glfwSetWindowMonitor(m_Window, glfwGetPrimaryMonitor(), 0, 0, 1920, 1080, 165);
		//glfwSwapInterval(0);

        return true;
    }

    void Core::Update()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (m_DrawDebugGUI)
            DrawDebugGUI();

        rgr::Time::CalcTime(glfwGetTime());

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (m_LoadedScene != nullptr) m_LoadedScene->Update();
        else
        {
            std::cout << "Active Scene pointer is set to nullptr!" << "\n";
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        ProcessInput();
        glfwSwapBuffers(m_Window);
        glfwPollEvents(); // This line should stay after the call to ProcessInput() function
    }

    void Core::LoadScene(rgr::Scene* scene)
    {
        if (scene == nullptr)
            return;

        m_LoadedScene = scene;
        m_RenderHandler->SetScene(m_LoadedScene);
    }

    bool Core::AppShouldRun()
    {
        return !glfwWindowShouldClose(m_Window);
    }

    void Core::Quit()
    {
        glfwTerminate();
    }

    glm::vec2 Core::GetWindowSize()
    {
        return {m_ScreenWidth, m_ScreenHeight};
    }

    void Core::OnScreenResize()
    {

    }

    void Core::ProcessInput()
    {
        rgr::Input::oldKeys.clear();
        rgr::Input::oldKeys.insert(rgr::Input::keys.begin(), rgr::Input::keys.end());

        rgr::Input::mouseDelta = rgr::Input::mousePos - rgr::Input::oldMousePos;
        rgr::Input::oldMousePos = rgr::Input::mousePos;
    }

    void Core::DrawDebugGUI()
    {
        const auto isSceneNotNull = m_LoadedScene != nullptr;

        const auto fpsText = "FPS: " + std::to_string(1 / rgr::Time::GetDeltaTimeF());
        const auto loadedSceneText = isSceneNotNull ? "Loaded Scene: " + m_LoadedScene->name : "Loaded Scene: None";
        const auto sceneObjectCountText = isSceneNotNull ? "Loaded Objs Count: " + std::to_string(m_LoadedScene->GetObjectsCount()) : "Loaded Objs Count: None";

        const size_t frametimeSamples = 100;
        static std::vector<float> frametimes;
        frametimes.push_back(rgr::Time::GetDeltaTimeF());
        if (frametimes.size() > frametimeSamples)
            frametimes.erase(frametimes.begin());

        ImGui::Begin("Debug GUI");

        ImGui::Text("%s", fpsText.c_str());
        ImGui::PlotLines("", frametimes.data(), static_cast<int>(frametimes.size()));
        ImGui::Text("%s", loadedSceneText.c_str());
        ImGui::Text("%s", sceneObjectCountText.c_str());

        ImGui::End();
    }

    void Core::EnableDebugGUI()
    {
        m_DrawDebugGUI = true;
    }

    void Core::DisableDebugGUI()
    {
        m_DrawDebugGUI = false;
    }

    void Core::framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        m_ScreenWidth = width;
        m_ScreenHeight = height;
        OnScreenResize();
    }

    void Core::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (action == GLFW_PRESS) Input::keys.insert(key);
        else if (action == GLFW_RELEASE)
        {
            Input::keys.erase(key);
        }
    }

    void Core::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
    {
        Input::mousePos = glm::vec2(xpos, ypos);
    }

    void Core::gl_debug_output_callback(uint32_t source, uint32_t type, uint32_t id,
                                        uint32_t severity, int length,
                                        const char *message, const void* userParam)
    {
        // ignore non-significant error/warning codes
        if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

        std::cout << "---------------" << std::endl;
        std::cout << "Debug message (" << id << "): " <<  message << std::endl;

        switch (source)
        {
            case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
            case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
            case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
            case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
            case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
        } std::cout << std::endl;

        switch (type)
        {
            case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
            case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
            case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
            case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
            case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
            case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
            case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
        } std::cout << std::endl;

        switch (severity)
        {
            case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
            case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
            case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
            case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
        } std::cout << std::endl;
        std::cout << std::endl;
    }

    void Core::RenderHandlerSceneUpdate()
    {
        m_RenderHandler->GenerateDepthMaps();
        m_RenderHandler->DoGeometryPass();
        m_RenderHandler->DoLightingPass();
        m_RenderHandler->DoForwardPass();
    }
}
