#pragma once

#include "objects/Object.hpp"
#include "internal.hpp"

namespace rgr
{	
	class Scene;

	// Base class for all objects able to render
	class Renderable : public rgr::Object
	{
	public:
		Renderable() { }
		virtual ~Renderable() { }
	INTERNAL:
		virtual void Render(const Scene* scene) = 0;
	};
}

