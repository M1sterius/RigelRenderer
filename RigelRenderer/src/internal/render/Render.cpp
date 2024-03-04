#include "glew.h"
#include "Render.hpp"
#include "components/Shader.hpp"
#include "glAbstraction/VertexArray.hpp"
#include "glAbstraction/IndexBuffer.hpp"

#include <vector>

rgr::RenderData::RenderData(rgr::Material* material, rgr::Mesh* mesh,
	const glm::mat4 mvp, const rgr::Camera::ViewMode viewMode, std::vector<rgr::Light*>* lights)
	: material(material), mesh(mesh), mvp(mvp), viewMode(viewMode), lights(lights)
{

}

rgr::RenderData::~RenderData()
{

}

static void SetLightUniforms(rgr::Shader* shader, std::vector<rgr::Light*>* lights)
{

}

void rgr::Render(const RenderData& data)
{
	rgr::Material* material = data.material;
	rgr::Shader* shader = material->GetShader();
	rgr::Mesh* mesh = data.mesh;
	glm::mat4 mvp = data.mvp;

	// Binding the vertex array as it needs to be bound to render any mesh type
	mesh->GetVertexArray()->Bind();

	// Binding the index buffer only if the mesh is indexed (uses index buffer)
	if (mesh->GetMeshType() != rgr::Mesh::MeshType::ARRAY)
		mesh->GetIndexBuffer()->Bind();

	/*
	Binding the material, calling the method that sets user defined uniforms, setting mvp
	Any uniform set errors that happen here will be caught and indicated by using placeholder shader
	*/
	material->Bind();
	material->SetUniforms();
	shader->SetUniformMat4("u_MVP", true, mvp);

	// Check if there were any errors in uniforms set process
	if (shader->GetUniformsCallback() != 0)
	{
		material->Unbind();
		rgr::Shader* pcShader = rgr::Shader::GetPlainColorShader();

		/*
		If there were any errors, than unbind material with errors and use placeholder shader
		Placeholder shader will draw the object with bright pink color indicating an error
		*/

		pcShader->Bind();
		pcShader->SetUniformMat4("u_MVP", true, mvp);
		pcShader->SetUniformVec4("u_Color", glm::vec4(0.96, 0.25, 0.89, 1.0));
	}

	/*
	If the camera is in wireframe mode, 
	than use the shader that will draw the wireframe with some plain color
	*/
	if (data.viewMode == rgr::Camera::ViewMode::WIREFRAME)
	{
		material->Unbind();
		rgr::Shader* pcShader = rgr::Shader::GetPlainColorShader();

		pcShader->Bind();
		pcShader->SetUniformMat4("u_MVP", true, mvp);
		pcShader->SetUniformVec4("u_Color", glm::vec4(0.0, 0.0, 0.0, 1.0));

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	// Use the correct OpenGL draw function depending on the mesh type
	if (mesh->GetMeshType() == rgr::Mesh::MeshType::INDEXED)
		glDrawElements(GL_TRIANGLES, mesh->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	else
		glDrawArrays(GL_TRIANGLES, 0, mesh->GetVertsCount());

	// Resetting the wireframe mode if it was used
	if (data.viewMode == rgr::Camera::ViewMode::WIREFRAME)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
