#pragma once

#include "mat4x4.hpp"
#include "Material.hpp"
#include "Mesh.hpp"
#include "Camera.hpp"
#include "Light.hpp"

namespace rgr
{	
	// Container for the data required to draw a 2D object
	struct RenderData2D
	{
		rgr::Material* material;
		rgr::Mesh* mesh;
		glm::mat4& mvp;
		rgr::Camera::ViewMode viewMode;

		RenderData2D(rgr::Material* material, rgr::Mesh* mesh, glm::mat4& mvp,
			const rgr::Camera::ViewMode viewMode);
		~RenderData2D();
	};

	// Container for the data required to draw a 3D object
	struct RenderData3D
	{
		rgr::Material* material;
		rgr::Mesh* mesh;
		glm::mat4& mvp;
		glm::mat4& model;
		glm::mat3& normal;
		rgr::Camera::ViewMode viewMode;
		glm::vec3 viewPos;
		std::vector<rgr::Light*>& lights;

		RenderData3D(rgr::Material* material, rgr::Mesh* mesh, glm::mat4& mvp,
			glm::mat4& model, glm::mat3& normal, const rgr::Camera::ViewMode viewMode,
			const glm::vec3 viewPos, std::vector<rgr::Light*>& lights);
		~RenderData3D();
	};

	// Draws a 2D object according to the specified data
	void Render2D(const RenderData2D& data);
	// Draws a 3D object according to the specified data
	void Render3D(const RenderData3D& data);
}
