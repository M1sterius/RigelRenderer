#include "Light.hpp"

namespace rgr
{
	Light::~Light()
	{

	}

	const glm::mat4 Light::GetLightSpaceView()
	{
		return glm::mat4(0.0f);
	}

	const glm::mat4 Light::GetLightSpaceViewProj()
	{
		return glm::mat4(0.0f);
	}

}
