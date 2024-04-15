#include "MaterialShadows.hpp"

#include "components/Shader.hpp"

MaterialShadows::MaterialShadows(rgr::Texture* texture, rgr::Shader* shader) : rgr::Material(shader)
{
	m_Texture = texture;

	MakeLit();
}

void MaterialShadows::SetUniforms()
{
	BindTexture("u_Texture", m_Texture, 0);
}

MaterialShadows::~MaterialShadows()
{

}
