#pragma once

#include "core/objects/Renderable.hpp"
#include "core/objects/Camera.hpp"
#include "core/objects/Light.hpp"

#include <vector>
#include <string>

namespace rgr
{	
	class Scene
	{
	private:
		std::vector<Renderable*> m_Renderables;
		std::vector<Camera*> m_Cameras;
		std::vector<Light*> m_Lights;

		rgr::Camera* GetMainCamera();
	public:
		std::string name = "Scene";

		Scene();
		~Scene();

		void Update();
		void AddObject(rgr::Object* object);
		void RemoveObject(rgr::Object* object);
	};
}

