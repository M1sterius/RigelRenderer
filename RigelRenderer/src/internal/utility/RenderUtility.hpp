#pragma once

#include "mat4x4.hpp"
#include "Material.hpp"
#include "Mesh.hpp"
#include "Camera.hpp"
#include "Lights/Light.hpp"
#include "Scene.hpp"

namespace rgr
{	
	void ProcessLighting(rgr::Shader* shader, const std::vector<rgr::Light*>& lights,
		const glm::vec3 viewPos, const glm::mat4& model, const glm::mat3& normal);

	// Sets shader to draw bright placeholder for shader with uniform set errors
	void DrawErrorPlaceholder(const glm::mat4& mvp);
	// Sets shader to draw object in wireframe mode
	void DrawWireframe(const glm::mat4& mvp);

	void DrawMesh(const rgr::Mesh* mesh);

	void ProcessShadowCasters(const Scene* scene);
}
