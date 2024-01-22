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

#define UR_OK 0

namespace rgr
{	
	int Init(int width, int height, const char* title);
	void SetScene(rgr::Scene* scene);
	void Update();
	void Quit();

	float GetTime();
	float GetDeltaTime();

	bool WindowShouldClose();
}