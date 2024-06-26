#pragma once

#include "objects/Object.hpp"
#include "glm.hpp"
#include "Internal.hpp"

namespace rgr
{
	class Light : public rgr::Object
	{
	public:
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
		float intensity = 1.0f;
		bool castShadows = true;

		virtual ~Light() { }

		virtual const glm::mat4 GetLightSpaceView() = 0;
		virtual const glm::mat4 GetLightSpaceViewProj() = 0;
	INTERNAL:
		virtual void GenerateDepthMap() = 0;
	};
}
