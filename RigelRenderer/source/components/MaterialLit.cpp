#include "MaterialLit.hpp"
#include "Shader.hpp"

namespace rgr
{
	MaterialLit::MaterialLit(rgr::Texture* diffuse)
    :   rgr::Material(rgr::Shader::GetBuiltInShader(rgr::Shader::BUILT_IN_SHADERS::LIGHTING_PASS)),
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
