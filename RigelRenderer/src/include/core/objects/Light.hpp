#pragma once

#include "Object.hpp"
#include "vec3.hpp"

namespace rgr
{
	class Light : public rgr::Object
	{
	public:
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
		float intensity = 1.0f;

		virtual ~Light() = 0;
	};
}
