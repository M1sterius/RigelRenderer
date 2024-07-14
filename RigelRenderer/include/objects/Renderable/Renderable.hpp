#pragma once

#include "objects/Object.hpp"
#include "Internal.hpp"
#include "glm.hpp"

namespace rgr
{	
	// Base class for all objects that can be rendered
	class Renderable : public rgr::Object
	{
	protected:
		Renderable() = default;
		virtual ~Renderable() = default;
	};
}

