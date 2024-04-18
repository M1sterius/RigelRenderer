#include "Object.hpp"
#include "Scene.hpp"

namespace rgr
{
	Object::Object()
	{
		m_Transform = rgr::Transform();
	}

	Object::Object(const rgr::Transform& transform)
	{
		m_Transform = rgr::Transform(transform);
	}

	Object::Object(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale)
	{
		m_Transform = rgr::Transform(position, rotation, scale);
	}

	Object::~Object()
	{
	}

	void Object::AssignScene(const rgr::Scene* scene)
	{
		m_ScenePtr = scene;
	}

	void Object::ResetScene()
	{
		m_ScenePtr = nullptr;
	}
}
