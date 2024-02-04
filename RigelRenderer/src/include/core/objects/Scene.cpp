#include "Scene.hpp"
#include "core/components/Material.hpp"
#include "core/components/Shader.hpp"

#include "glAbstraction/GlAbstraction.hpp"
#include "gtx/string_cast.hpp"
#include "glm.hpp"
#include "glfw3.h"
#include "glew.h"

#include "render/Render.hpp"

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

	void Scene::Update()
	{	
		#pragma region Cameras 

		rgr::Camera* camera = nullptr;
		camera = GetMainCamera();
		if (camera == nullptr) return;

		glm::mat4 view = camera->GetView();
		glm::mat4 perspProj = camera->GetPerspective();
		glm::mat4 orthoProj = camera->GetOrthographic();

		#pragma endregion

		#pragma region Lights
		#pragma endregion

		#pragma region Render

		for (const auto& renderable : m_Renderables)
		{
			rgr::Transform transform = renderable->GetTransform();
			rgr::Material* material = renderable->GetMaterial();
			rgr::Shader* shader = material->GetShader();
			rgr::Mesh* mesh = renderable->GetMesh();
			glm::mat4 model = transform.GetModelMatrix();

			switch (transform.space)
			{
				case rgr::Transform::Space::WORLD_3D:
				{
					RenderData data = RenderData(
						material,
						mesh,
						perspProj * view * model,
						camera->viewMode
					);
					//data.material = material;
					//data.mesh = mesh;
					//data.mvp = perspProj * view * model;
					//data.viewMode = camera->viewMode;

					rgr::Render(data);
					
					break;
				}
				case rgr::Transform::Space::SCREEN_2D:
				{
					RenderData data = RenderData(
						material,
						mesh,
						orthoProj * model,
						camera->viewMode
					);

					rgr::Render(data);

					break;
				}
				default:
				{
					std::cout << "Invalid transform space of the Object named: " << renderable->name << '\n';
				}
			}
		}

		#pragma endregion
	}
	void Scene::AddObject(rgr::Object* object)
	{
		if (rgr::Renderable* renderablePtr = dynamic_cast<rgr::Renderable*>(object))
		{
			m_Renderables.push_back(renderablePtr);
		}
		else if (rgr::Camera* cameraIteratedPtr = dynamic_cast<rgr::Camera*>(object))
		{
			m_Cameras.push_back(cameraIteratedPtr);
		}
		else if (rgr::Light* lightPtr = dynamic_cast<rgr::Light*>(object))
		{
			m_Lights.push_back(lightPtr);
		}
		else
		{
			std::cout << "Invalid Object pointer type!" << '\n';
		}
	}
	void Scene::RemoveObject(rgr::Object* object)
	{
		if (rgr::Renderable* renderablePtr = dynamic_cast<rgr::Renderable*>(object))
		{
			auto iterator = std::find(m_Renderables.begin(), m_Renderables.end(), renderablePtr);
			if (iterator != m_Renderables.end())
				m_Renderables.erase(iterator);
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

	rgr::Camera* Scene::GetMainCamera()
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
}
