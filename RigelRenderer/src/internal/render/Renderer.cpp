#include "Renderer.hpp"
#include "GBuffer.hpp"
#include "lights/DirectionalLight.hpp"

#include "glew.h"
#include "Scene.hpp"

namespace rgr
{
	void Renderer::DrawMesh(const Mesh* mesh)
	{
		if (mesh->GetMeshType() == rgr::Mesh::MeshType::INDEXED)
			glDrawElements(GL_TRIANGLES, mesh->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
		else
			glDrawArrays(GL_TRIANGLES, 0, mesh->GetVertsCount());
	}

	void Renderer::GenerateDepthMapsForLightSources(const Scene* scene)
	{
		static unsigned int depthMapFBO = 0;

		// Pre-setup depth map framebuffer for future depth map generations only if it hasn't been set up yet
		if (depthMapFBO == 0)
		{
			// Note that attaching a depth map is skipped, because one will later be attached for each light individually
			glGenFramebuffers(1, &depthMapFBO);
			glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		rgr::Camera* camera = scene->GetMainCamera();
		const auto& lights = scene->GetLightsAround(camera->GetTransform().GetPosition(), camera->shadowsVisibilityDistance);

		for (size_t i = 0; i < lights.size(); i++)
		{
			rgr::Light* light = lights[i];

			if (!light->castShadows) continue;

			light->GenerateDepthMap(depthMapFBO);

			//Render the depth map of the last directional light to a quad for debug
			//if (static_cast<DirectionalLight*>(light) != nullptr)
			//{
			//	rgr::Mesh* quad = rgr::Mesh::Get2DQuadMesh();
			//	rgr::Shader* testShader = rgr::Shader::GetDepthTestShader();

			//	quad->GetVertexArray()->Bind();
			//	quad->GetIndexBuffer()->Bind();

			//	testShader->Bind();
			//	glActiveTexture(GL_TEXTURE0);
			//	glBindTexture(GL_TEXTURE_2D, light->GetDepthMapHandle());
			//	testShader->SetUniform1i("u_DepthMap", light->GetDepthMapHandle());

			//	DrawMesh(quad);
			//}
		}
	}

	void Renderer::DoGeometryPass(const Scene* scene, const GBuffer& gBuffer)
	{

	}

	void Renderer::DoLightingPass(const Scene* scene, const GBuffer& gBuffer)
	{

	}

}