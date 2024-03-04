#include "Material3D.hpp"

#include "components/Shader.hpp"

Material3D::Material3D(rgr::Texture* texture, rgr::Shader* shader) : rgr::Material(shader)
{
	m_Texture = texture;
}

void Material3D::SetUniforms()
{
	BindTexture("u_Texture", m_Texture, 0);
}

Material3D::~Material3D()
{

}
