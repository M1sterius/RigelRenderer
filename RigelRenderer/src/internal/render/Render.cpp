#include "glew.h"
#include "Render.hpp"
#include "components/Shader.hpp"
#include "glAbstraction/GlAbstraction.hpp"
#include "Light.hpp"
#include "DirectionalLight.hpp"
#include "PointLight.hpp"
#include "SpotLight.hpp"

#include <vector>
#include <string>

rgr::RenderData2D::RenderData2D(rgr::Material* material, rgr::Mesh* mesh, glm::mat4& mvp,
	const rgr::Camera::ViewMode viewMode)
	: material(material), mesh(mesh), mvp(mvp), viewMode(viewMode)
{
}

rgr::RenderData2D::~RenderData2D()
{

}

rgr::RenderData3D::RenderData3D(rgr::Material* material, rgr::Mesh* mesh, glm::mat4& mvp,
	glm::mat4& model, glm::mat3& normal, const rgr::Camera::ViewMode viewMode,
	const glm::vec3 viewPos, std::vector<rgr::Light*>& lights)
	: material(material), mesh(mesh), mvp(mvp), model(model), normal(normal), viewMode(viewMode), viewPos(viewPos), lights(lights)
{
}

rgr::RenderData3D::~RenderData3D()
{

}

static void ProcessLighting(rgr::Shader* shader, std::vector<rgr::Light*>& lights, 
	const glm::vec3 viewPos, glm::mat4& model, glm::mat3& normal)
{	
	unsigned int dirCount = 0;
	unsigned int pointCount = 0;
	unsigned int spotCount = 0;

	// Iterating through all the lights affecting this object and setting corresponding uniforms
	for (rgr::Light* light : lights)
	{	
		if (dynamic_cast<rgr::DirectionalLight*>(light) != nullptr) 
		{	
			rgr::DirectionalLight* dirLight = dynamic_cast<rgr::DirectionalLight*>(light);

			std::string uName = "u_DirectionalLights[" + std::to_string(dirCount) + "].";
			shader->SetUniformVec3(uName + "color", dirLight->color);
			shader->SetUniform1f(uName + "intensity", dirLight->intensity);
			shader->SetUniformVec3(uName + "direction", dirLight->direction);

			dirCount++;
		}
		if (dynamic_cast<rgr::PointLight*>(light) != nullptr) 
		{	
			rgr::PointLight* pointLight = dynamic_cast<rgr::PointLight*>(light);

			std::string uName = "u_PointLights[" + std::to_string(pointCount) + "].";
			shader->SetUniformVec3(uName + "color", pointLight->color);
			shader->SetUniformVec3(uName + "position", pointLight->GetTransform().GetPosition());
			shader->SetUniform1f(uName + "intensity", pointLight->intensity);
			shader->SetUniform1f(uName + "constant", pointLight->constant);
			shader->SetUniform1f(uName + "linear", pointLight->linear);
			shader->SetUniform1f(uName + "quadratic", pointLight->quadratic);

			pointCount++;
		}
		else if (dynamic_cast<rgr::SpotLight*>(light) != nullptr) 
		{	
			rgr::SpotLight* spotLight = dynamic_cast<rgr::SpotLight*>(light);

			std::string uName = "u_SpotLights[" + std::to_string(spotCount) + "].";
			shader->SetUniformVec3(uName + "color", spotLight->color);
			shader->SetUniformVec3(uName + "position", spotLight->GetTransform().GetPosition());
			shader->SetUniformVec3(uName + "direction", spotLight->direction);
			shader->SetUniform1f(uName + "intensity", spotLight->intensity);
			shader->SetUniform1f(uName + "constant", spotLight->constant);
			shader->SetUniform1f(uName + "linear", spotLight->linear);
			shader->SetUniform1f(uName + "quadratic", spotLight->quadratic);
			shader->SetUniform1f(uName + "cutOff", spotLight->cutOff);
			shader->SetUniform1f(uName + "outerCutOff", spotLight->outerCutOff);

			spotCount++;
		}
	}

	shader->SetUniformVec3("u_ViewPos", viewPos);
	shader->SetUniformMat4("u_Model", false, model);
	shader->SetUniformMat3("u_NormalMatrix", true, normal);

	shader->SetUniform1i("u_DirectionalLightsCount", dirCount);
	shader->SetUniform1i("u_PointLightsCount", pointCount);
	shader->SetUniform1i("u_SpotLightsCount", spotCount);
}

void rgr::Render2D(const RenderData2D& data)
{
	rgr::Material* material = data.material;
	rgr::Shader* shader = material->GetShader();
	rgr::Mesh* mesh = data.mesh;
	glm::mat4 mvp = data.mvp;

	// Binding the vertex array as it needs to be bound to render any mesh type
	mesh->GetVertexArray()->Bind();

	// Binding the index buffer only if the mesh is indexed (uses index buffer)
	if (mesh->GetMeshType() == rgr::Mesh::MeshType::INDEXED)
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
		rgr::Shader* phShader = rgr::Shader::GetPlainColorShader();

		/*
		If there were any errors, then unbind material with errors and use placeholder shader
		Placeholder shader will draw the object with bright pink color indicating an error
		*/

		phShader->Bind();
		phShader->SetUniformMat4("u_MVP", true, mvp);
		phShader->SetUniformVec4("u_Color", glm::vec4(0.96, 0.25, 0.89, 1.0));
	}

	/*
	If the camera is in the wireframe mode,
	use the shader that will draw the wireframe with some plain color
	*/
	if (data.viewMode == rgr::Camera::ViewMode::WIREFRAME)
	{
		material->Unbind();
		rgr::Shader* phShader = rgr::Shader::GetPlainColorShader();

		phShader->Bind();
		phShader->SetUniformMat4("u_MVP", true, mvp);
		phShader->SetUniformVec4("u_Color", glm::vec4(0.0, 0.0, 0.0, 1.0));

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

void rgr::Render3D(const RenderData3D& data)
{
	rgr::Material* material = data.material;
	rgr::Shader* shader = material->GetShader();
	rgr::Mesh* mesh = data.mesh;
	glm::mat4 mvp = data.mvp;
	glm::mat4 model = data.model;
	glm::mat3 normal = data.normal;
	std::vector<rgr::Light*> lights = data.lights;
	glm::vec3 viewPos = data.viewPos;

	// Binding the vertex array as it needs to be bound to render any mesh type
	mesh->GetVertexArray()->Bind();

	// Binding the index buffer only if the mesh is indexed (uses index buffer)
	if (mesh->GetMeshType() == rgr::Mesh::MeshType::INDEXED)
		mesh->GetIndexBuffer()->Bind();

	/*
	Binding the material, calling the method that sets user defined uniforms, setting mvp
	Any uniform set errors that happen here will be caught and indicated by using placeholder shader
	*/
	material->Bind();
	material->SetUniforms();
	shader->SetUniformMat4("u_MVP", true, mvp);

	if (material->GetIsLit())
		ProcessLighting(shader, lights, viewPos, model, normal);

	// Check if there were any errors in uniforms set process
	if (shader->GetUniformsCallback() != 0)
	{
		material->Unbind();
		rgr::Shader* phShader = rgr::Shader::GetPlainColorShader();

		/*
		If there were any errors, then unbind material with errors and use placeholder shader
		Placeholder shader will draw the object with bright pink color indicating an error
		*/

		phShader->Bind();
		phShader->SetUniformMat4("u_MVP", true, mvp);
		phShader->SetUniformVec4("u_Color", glm::vec4(0.96, 0.25, 0.89, 1.0));
	}

	/*
	If the camera is in the wireframe mode,
	use the shader that will draw the wireframe with some plain color
	*/
	if (data.viewMode == rgr::Camera::ViewMode::WIREFRAME)
	{
		material->Unbind();
		rgr::Shader* phShader = rgr::Shader::GetPlainColorShader();

		phShader->Bind();
		phShader->SetUniformMat4("u_MVP", true, mvp);
		phShader->SetUniformVec4("u_Color", glm::vec4(0.0, 0.0, 0.0, 1.0));

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
