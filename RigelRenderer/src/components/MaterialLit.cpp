#include "MaterialLit.hpp"
#include "Shader.hpp"

namespace rgr
{
	MaterialLit::MaterialLit(rgr::Texture* diffuse) : rgr::Material(rgr::Shader::GetLightingPassShader()),
		m_DiffuseTexture(diffuse)
	{
		
	}

	MaterialLit::~MaterialLit()
	{

	}

	void MaterialLit::SetUniforms()
	{

	}

}
