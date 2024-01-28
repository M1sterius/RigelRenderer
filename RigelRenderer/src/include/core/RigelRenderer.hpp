#pragma once

#include "components/Transform.hpp"
#include "components/Mesh.hpp"
#include "components/Material.hpp"
#include "components/Shader.hpp"
#include "components/Texture.hpp"

#include "objects/Scene.hpp"
#include "objects/Object.hpp"
#include "objects/Renderable.hpp"
#include "objects/Camera.hpp"
#include "objects/Light.hpp"
#include "objects/PointLight.hpp"
#include "objects/DirectionalLight.hpp"
#include "objects/SpotLight.hpp"

#include "input/Input.hpp"

#define RIGEL_OK 0

namespace rgr
{	
	/*
	Initializes glfw, glew, rendering context and internal library modules,
	as well as creates the window of given size and title.
	Returns RIGEL_OK if the initialization was successful
	*/
	int Init(int width, int height, const char* title);
	/*
	Makes the scene at given pointer active so it will be rendered and processed in rgr::Update()
	*/
	void SetScene(rgr::Scene* scene);
	/*
	Main game loop function that renderes scene, processes events and updates all library modules
	*/
	void Update();
	/*
	Used after the game loop to properly terminate the game
	*/
	void Quit();

	/*
	The time passed since rgr::Init() was called
	*/
	float GetTime();
	/*
	The time passed bewteen two previous game loop iterations
	*/
	float GetDeltaTime();

	/*
	True if the window was closed by player
	*/
	bool WindowShouldClose();
}