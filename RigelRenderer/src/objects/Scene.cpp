#include "Scene.hpp"
#include "components/Material.hpp"
#include "components/Shader.hpp"
#include "objects/Lights/DirectionalLight.hpp"
#include "glAbstraction/GlAbstraction.hpp"

#include "RenderUtility.hpp"
#include "Logger.hpp"

#include "gtx/string_cast.hpp"
#include "glm.hpp"
#include "glfw3.h"
#include "glew.h"

//#define GLT_IMPLEMENTATION
//#include "gltext.h"

#include <iostream>

namespace rgr
{
	Scene::Scene()
	{

	}

	Scene::~Scene()
	{

	}

	rgr::Camera* Scene::FindMainCamera() const
	{
		bool noMainCameraFlag = true;
		for (const auto& cameraIterated : m_Cameras)
		{
			if (cameraIterated->IsMain())
			{
				noMainCameraFlag = false;
				return cameraIterated;
			}
		}
		if (noMainCameraFlag)
		{
			if (m_Cameras.size() > 0) return m_Cameras[0];
			else
			{
				std::cout << "Scene '" << this->name << "' does not contain a suitable rendering camera!" << '\n';
				return nullptr;
			}
		}
	}

	void Scene::Update()
	{	
		// Make sure that there always is a suitable rendering camera
		m_MainCamera = FindMainCamera();
		if (m_MainCamera == nullptr)
			return;

		// Generates depth maps for all lights close to camera
		rgr::ProcessShadowCasters(this);

		return;

		for (size_t i = 0; i < m_Renderables.size(); i++)
		{
			rgr::Renderable* renderable = m_Renderables[i];
			renderable->Render();
		}
	}

	void Scene::AddObject(rgr::Object* object)
	{
		if (rgr::Renderable* renderablePtr = dynamic_cast<rgr::Renderable*>(object))
		{
			m_Renderables.push_back(renderablePtr);
		}
		else if (rgr::Camera* cameraPtr = dynamic_cast<rgr::Camera*>(object))
		{
			m_Cameras.push_back(cameraPtr);
		}
		else if (rgr::Light* lightPtr = dynamic_cast<rgr::Light*>(object))
		{
			m_Lights.push_back(lightPtr);
		}
		else
		{
			std::cout << "Invalid Object pointer type!" << '\n';
			return;
		}

		object->AssignScene(this);
	}
	void Scene::RemoveObject(rgr::Object* object)
	{
		if (rgr::Renderable* renderablePtr = dynamic_cast<rgr::Renderable*>(object))
		{
			auto iterator = std::find(m_Renderables.begin(), m_Renderables.end(), renderablePtr);
			if (iterator != m_Renderables.end())
			{
				m_Renderables.erase(iterator);
				object->ResetScene();
			}
			else
			{
				std::cout << "Unable to find a Renderable by given pointer!" << '\n';
			}
		}
		else if (rgr::Camera* cameraIteratedPtr = dynamic_cast<rgr::Camera*>(object))
		{
			auto iterator = std::find(m_Cameras.begin(), m_Cameras.end(), cameraIteratedPtr);
			if (iterator != m_Cameras.end())
				m_Cameras.erase(iterator);
			else
			{
				std::cout << "Unable to find a Camera by given pointer!" << '\n';
			}
		}
		else if (rgr::Light* lightPtr = dynamic_cast<rgr::Light*>(object))
		{
			auto iterator = std::find(m_Lights.begin(), m_Lights.end(), lightPtr);
			if (iterator != m_Lights.end())
				m_Lights.erase(iterator);
			else
			{
				std::cout << "Unable to find a Renderable by given pointer!" << '\n';
			}
		}
		else
		{
			std::cout << "Invalid Object pointer type!" << '\n';
		}
	}

	rgr::Camera* Scene::GetMainCamera() const
	{
		return m_MainCamera;
	}

	const std::vector<Light*>& Scene::GetLightsAround(const glm::vec3 point, const float radius, const size_t maxCount /*=16*/) const
	{
		static std::vector<Light*> lights(16);

		lights.clear();

		for (size_t i = 0; i < m_Lights.size(); i++)
		{	
			if (lights.size() >= maxCount) return lights;

			Light* light = m_Lights[i];

			if (glm::distance(light->GetTransform().GetPosition(), point) < radius ||
				static_cast<DirectionalLight*>(light) != nullptr) // Always add all directional lights
			{
				lights.push_back(light);
			}
		}

		return lights;
	}

	const std::vector<Renderable*>& Scene::GetObjectsInFrustrum() const
	{
		return m_Renderables;
	}

}
