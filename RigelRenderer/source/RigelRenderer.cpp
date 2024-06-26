#include "glAbstraction/GlAbstraction.hpp"
#include "utility/InputUtility.hpp"
#include "RigelRenderer.hpp"
#include "glfw3.h"
#include "glew.h"

#include "Logger.hpp"

#include <map>
#include <iostream>

namespace rgr
{
	static GLFWwindow* window;
	static Scene* currentScene;

	static float oldTime;
	static float deltaTime;

	static size_t viewportWidth;
	static size_t viewportHeight;

	static void ChangeViewport(const size_t newWidth, const size_t newHeight)
	{
		viewportWidth = newWidth;
		viewportHeight = newHeight;
		glViewport(0, 0, viewportWidth, viewportHeight);
	}

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		ChangeViewport(width, height);
	}

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (action == GLFW_PRESS) InputUtility::keys.insert(key);
		else if (action == GLFW_RELEASE)
		{
			InputUtility::keys.erase(key);
		}
	}
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
	{
		InputUtility::mousePos = glm::vec2(xpos, ypos);
	}

	static void CalculateDeltaTime()
	{
		deltaTime = GetTimePassed() - oldTime;
		oldTime = GetTimePassed();
	}

	int Init(size_t width, size_t height, const char* title)
	{
		if (!glfwInit())
			return -1;

		window = glfwCreateWindow(width, height, title, nullptr, nullptr);
		if (!window)
		{
			glfwTerminate();
			return -1;
		}

		glfwMakeContextCurrent(window);

		glfwSetKeyCallback(window, key_callback);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		glfwSetCursorPosCallback(window, cursor_position_callback);

		if (glewInit() != GLEW_OK)
			return -1;

		// Blending must be disabled during deferred rendering

		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
		//glEnable(GL_BLEND);

		ChangeViewport(width, height);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		//glfwSetWindowAttrib(window, GLFW_RESIZABLE, GLFW_FALSE);
		//glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, 1920, 1080, 165);
		//glfwSwapInterval(0);

		return RIGEL_OK;
	}

	void SetScene(rgr::Scene* scene)
	{
		currentScene = scene;
	}

	void Update()
	{
		CalculateDeltaTime();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (currentScene != nullptr) currentScene->Update();
		else
		{
			std::cout << "Active Scene pointer is set to nullptr!" << "\n";
		}

		InputUtility::oldKeys.clear();
		InputUtility::oldKeys.insert(InputUtility::keys.begin(), InputUtility::keys.end());

		InputUtility::mouseDelta = InputUtility::mousePos - InputUtility::oldMousePos;
		InputUtility::oldMousePos = InputUtility::mousePos;

		glfwSwapBuffers(window);
		glfwPollEvents(); // Should stay the last line in Update
	}

	double GetTimePassed() { return glfwGetTime(); }

	float GetDeltaTime() { return deltaTime; }

	bool WindowShouldClose() { return glfwWindowShouldClose(window); }

	void Quit() { glfwTerminate(); }

	rgr::ViewportSize GetViewportSize()
	{
		return ViewportSize{ viewportWidth, viewportHeight };
	}
}
