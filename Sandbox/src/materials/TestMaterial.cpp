#include "TestMaterial.hpp"

#include "core/components/Shader.hpp"

TestMaterial::TestMaterial(rgr::Texture* texture) : rgr::Material()
{	
	m_Texture = texture;
	m_Shader = rgr::Shader::FromFiles("resources/shaders/vertex.glsl", "resources/shaders/fragment.glsl");
}

void TestMaterial::SetUniforms()
{
	BindTexture("u_Texture", m_Texture, 0);
}

TestMaterial::~TestMaterial()
{
	delete m_Shader;
}
