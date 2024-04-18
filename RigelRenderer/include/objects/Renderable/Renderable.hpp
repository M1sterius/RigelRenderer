#pragma once

#include "objects/Object.hpp"
#include "internal.hpp"
#include "glm.hpp"

namespace rgr
{	
	// Base class for all objects that can be rendered
	class Renderable : public rgr::Object
	{
	public:
		Renderable() { }
		virtual ~Renderable() { }
		bool shadowCaster = true;
	INTERNAL:
		virtual void Render() = 0;
		// Defines how this renderable will be rendered to a depth map
		// (Leave blank if this renderable should not emit shadows)
		virtual void DepthRender(const glm::mat4& lightSpaceMatrix) { }
	};
}

