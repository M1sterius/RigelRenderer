#pragma once

#include <vector>
#include <string>
#include <memory>

namespace rgr
{
    class RenderHandler;
	class GBuffer;
    class Renderable;
    class Camera;
    class Light;
    class Object;

	/*
	Represents the collection of objects that form a virtual environment
	*/
	class Scene
	{
	private:
		std::vector<std::shared_ptr<Renderable>> m_Renderables;
		std::vector<std::shared_ptr<Camera>> m_Cameras;
		std::vector<std::shared_ptr<Light>> m_Lights;

        std::unique_ptr<RenderHandler> m_RenderHandler;

		std::shared_ptr<Camera> m_MainCamera = nullptr;
        std::shared_ptr<Camera> FindMainCamera() const;
	public:
		/*
		Publicly visible and modifiable name used to distinguish scenes from one another
		*/
		std::string name = "Scene";

		Scene();
		~Scene();

		/*
		Performs all graphics calculations and renders the scene to the screen 
		(must not be called outside rgr::Update)
		*/
		void Update();
		/*
		Adds Renderable, Camera or Light Object to the scene.
		Note that the scene contains raw pointers to objects created outside the library
		*/
		void AddObject(std::shared_ptr<Object> object);
		/*
		Removes Renderable, Camera or Light Object from the scene.
		Note that this method DOES NOT delete an object entirely, it just 
		removes its pointer from the scene
		*/
		void RemoveObject(std::shared_ptr<Object> object);

        inline size_t GetObjectsCount() const {return m_Renderables.size() + m_Cameras.size() + m_Lights.size(); }

        // Helps find a suitable rendering camera
        std::shared_ptr<Camera> GetMainCamera() const;
    INTERNAL:
		// Returns all lights closer than radius around the point, up to maxCount of lights
		const std::vector<std::shared_ptr<Light>>& GetLightsAround(const glm::vec3 point, const float radius, const size_t maxCount = 16) const;
		
		const std::vector<std::shared_ptr<Renderable>>& GetRenderablesInFrustum() const; // TODO: Add the frustum culling itself

		const std::vector<std::shared_ptr<Renderable>>& GetRenderablesByCondition(bool(*func)(const std::shared_ptr<Renderable>&), const size_t maxCount = 64) const;
	};
}

