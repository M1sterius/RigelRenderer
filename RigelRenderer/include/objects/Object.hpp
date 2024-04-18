#pragma once

#include "components/Transform.hpp"
#include "internal.hpp"

#include <string>

namespace rgr
{	
	class Scene;

	class Object
	{	
	public:
		std::string name = "Object";
		inline rgr::Transform& GetTransform() { return m_Transform; }
	protected:
		Object();
		Object(const rgr::Transform& transform);
		Object(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale);
		virtual ~Object();

		const rgr::Scene* m_ScenePtr = nullptr;
	private:
		rgr::Transform m_Transform;
	INTERNAL:
		// Sets the pointer to the scene this object is on
		void AssignScene(const rgr::Scene* scene);
		// Resets the pointer to the scene when this object is removed from it
		void ResetScene();
	};
}