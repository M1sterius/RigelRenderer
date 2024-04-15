#pragma once

#include "Object.hpp"
#include "glm.hpp"

namespace rgr
{
	class Light : public rgr::Object
	{
	public:
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
		float intensity = 1.0f;
		bool castShadows = true;

		virtual ~Light();

		virtual const glm::mat4 GetLightSpaceView();
		virtual const glm::mat4 GetLightSpaceViewProj();
	};
}
