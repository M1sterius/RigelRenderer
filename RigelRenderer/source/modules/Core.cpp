#include "Core.hpp"
#include "Input.hpp"
#include "Time.hpp"
#include "Scene.hpp"
#include "glad.h"
#include "glfw3.h"

#include <iostream>

namespace rgr
{
    GLFWwindow* Core::m_Window = nullptr;
    rgr::Scene* Core::m_LoadedScene = nullptr;
    size_t Core::m_ScreenWidth = 0;
    size_t Core::m_ScreenHeight = 0;

    bool Core::Init(size_t width, size_t height, const char* title)
    {
        if (!glfwInit())
            return false;

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

        glViewport(0, 0, static_cast<int>(m_ScreenWidth), static_cast<int>(m_ScreenHeight));
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		glfwSetWindowAttrib(m_Window, GLFW_RESIZABLE, GLFW_FALSE);
//		glfwSetWindowMonitor(m_Window, glfwGetPrimaryMonitor(), 0, 0, 1920, 1080, 165);
//		glfwSwapInterval(0);

        return true;
    }

    void Core::Update()
    {
        rgr::Time::CalcTime(glfwGetTime());

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (m_LoadedScene != nullptr) m_LoadedScene->Update();
        else
        {
            std::cout << "Active Scene pointer is set to nullptr!" << "\n";
        }

        ProcessInput();
        glfwSwapBuffers(m_Window);
        glfwPollEvents(); // Should stay the last line in Update
    }

    void Core::LoadScene(rgr::Scene* scene)
    {
        m_LoadedScene = scene;
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
}
