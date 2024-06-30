#pragma once

#include "components/Transform.hpp"
#include "components/Mesh.hpp"
#include "components/Material.hpp"
#include "components/MaterialLit.hpp"
#include "components/Shader.hpp"
#include "components/Texture.hpp"

#include "objects/Scene.hpp"
#include "objects/Object.hpp"
#include "objects/Camera.hpp"
#include "objects/lights/Light.hpp"
#include "objects/lights/PointLight.hpp"
#include "objects/lights/DirectionalLight.hpp"
#include "objects/lights/SpotLight.hpp"

#include "objects/renderable/Renderable.hpp"
#include "objects/renderable/RenderableMesh.hpp"

#include "input/Input.hpp"

#define RIGEL_OK 0

namespace rgr
{	
	struct ViewportSize
	{
		size_t width;
		size_t height;
	};

	/*
	Initializes glfw, glew, rendering context and internal library modules,
	as well as creates the window of given size and title.
	Returns RIGEL_OK if the initialization was successful
	*/
	int Init(size_t width, size_t height, const char* title);

	// Makes the scene at given pointer active, so it will be rendered and processed in rgr::Update()
	void SetScene(rgr::Scene* scene);

	// Main game loop function that renders scene, processes events and updates all library modules
	void Update();

	// Used after the game loop to properly terminate the game
	void Quit();

	// The time passed since rgr::Init() was called
	double GetTimePassed();

	// The time passed between two previous game loop iterations
	float GetDeltaTime();

	// True if the window was closed by player
	bool WindowShouldClose();

	// Returns the size of the viewport
	ViewportSize GetViewportSize();
}
