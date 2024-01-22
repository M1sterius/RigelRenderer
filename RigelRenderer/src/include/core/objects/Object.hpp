#pragma once

#include "core\components\Transform.hpp"

#include <string>

namespace rgr
{
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
	private:
		rgr::Transform m_Transform;
	};
}