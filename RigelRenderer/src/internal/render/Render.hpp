#pragma once

#include "mat4x4.hpp"

namespace rgr
{
	class Mesh;
	class Material;

	void Render(rgr::Mesh* mesh, rgr::Material* material, const glm::mat4& mvp);
}