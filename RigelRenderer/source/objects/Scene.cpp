#include "RigelRenderer.hpp"
#include "glAbstraction/GlAbstraction.hpp"
#include "render/GBuffer.hpp"
#include "render/Renderer.hpp"
#include "renderable/CustomRenderable.hpp"

#include "glm.hpp"
#include "glfw3.h"
#include "Scene.hpp"


#include <iostream>
#include <algorithm>

namespace rgr
{
	Scene::Scene() = default;
	Scene::~Scene() = default;

	std::shared_ptr<Camera> Scene::FindMainCamera() const
	{
		for (const auto& camera : m_Cameras)
		{
			if (camera->IsMain())
                return camera;
		}
        if (!m_Cameras.empty()) return m_Cameras[0];
        else
        {
            std::cout << "Scene '" << this->name << "' does not contain a suitable rendering camera!" << '\n';
            return nullptr;
        }
	}

	void Scene::Update()
	{	
		// Make sure that there is always a suitable rendering camera
		m_MainCamera = FindMainCamera();
		if (m_MainCamera == nullptr)
			return;

        rgr::Core::GetRendererInstance()->RenderScene(this);
	}

	void Scene::AddObject(std::shared_ptr<Object> object)
	{
		if (auto renderablePtr = std::dynamic_pointer_cast<rgr::Renderable>(object))
		{
			m_Renderables.push_back(renderablePtr);
		}
		else if (auto cameraPtr = std::dynamic_pointer_cast<rgr::Camera>(object))
		{
			m_Cameras.push_back(cameraPtr);
		}
		else if (auto lightPtr = std::dynamic_pointer_cast<rgr::Light>(object))
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
	void Scene::RemoveObject(std::shared_ptr<Object> object)
	{
		if (auto renderablePtr = std::dynamic_pointer_cast<rgr::Renderable>(object))
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
		else if (auto cameraIteratedPtr = std::dynamic_pointer_cast<rgr::Camera>(object))
		{
			auto iterator = std::find(m_Cameras.begin(), m_Cameras.end(), cameraIteratedPtr);
			if (iterator != m_Cameras.end())
				m_Cameras.erase(iterator);
			else
			{
				std::cout << "Unable to find a Camera by given pointer!" << '\n';
			}
		}
		else if (auto lightPtr = std::dynamic_pointer_cast<rgr::Light>(object))
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

    std::shared_ptr<Camera> Scene::GetMainCamera() const
	{
		return m_MainCamera;
	}

	const std::vector<std::shared_ptr<Light>>& Scene::GetLightsAround(const glm::vec3 point, const float radius, const size_t maxCount /*=16*/) const
	{
		static std::vector<std::shared_ptr<Light>> lights(16);

		lights.clear();

		for (size_t i = 0; i < m_Lights.size(); i++)
		{	
			if (lights.size() >= maxCount) return lights;

			std::shared_ptr<Light> light = m_Lights[i];

			if (glm::distance(light->GetTransform().GetPosition(), point) < radius ||
                    std::dynamic_pointer_cast<DirectionalLight>(light) != nullptr) // Always add all directional lights
			{
				lights.push_back(light);
			}
		}

		return lights;
	}

	const std::vector<std::shared_ptr<Renderable>>& Scene::GetAllRenderables() const
	{
		return m_Renderables;
	}

	const std::vector<std::shared_ptr<Renderable>>& Scene::GetRenderablesByCondition(bool(*func)(const std::shared_ptr<Renderable>&), const size_t maxCount /*= 64*/) const
	{
		static std::vector<std::shared_ptr<Renderable>> renderables(64);
		renderables.clear();

		for (const auto& renderable : m_Renderables)
		{
            if (func(renderable))
                renderables.push_back(renderable);
		}

		return renderables;
	}

    size_t Scene::GetVerticesCount() const
    {
        size_t count = 0;
        for (auto& obj : m_Renderables)
        {
            if (auto model = std::static_pointer_cast<rgr::RenderableModel>(obj))
                count += model->GetModel()->GetVerticesCount();
        }
        return count;
    }

    size_t Scene::GetTrianglesCount() const
    {
        size_t count = 0;
        for (auto& obj : m_Renderables)
        {
            if (auto model = std::static_pointer_cast<rgr::RenderableModel>(obj))
                count += model->GetModel()->GetTrianglesCount();
        }
        return count;
    }
}
