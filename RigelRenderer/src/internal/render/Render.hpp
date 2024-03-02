#pragma once

#include "mat4x4.hpp"
#include "core/components/Material.hpp"
#include "core/components/Mesh.hpp"
#include "core/objects/Camera.hpp"
#include "core/objects/Light.hpp"

namespace rgr
{	
	/*
	The container for data required to make a single draw call
	*/
	struct RenderData
	{
		rgr::Material* material;
		rgr::Mesh* mesh;
		glm::mat4 mvp;
		rgr::Camera::ViewMode viewMode;
		std::vector<rgr::Light*>* lights;

		RenderData(rgr::Material* material, rgr::Mesh* mesh, 
			const glm::mat4 mvp, const rgr::Camera::ViewMode viewMode, std::vector<rgr::Light*>* lights);
		~RenderData();
	};

	/*
	Draws an object on the screen using the sprecified data
	*/
	void Render(const RenderData& data);
}