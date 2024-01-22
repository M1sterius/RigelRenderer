#include "Material3D.hpp"

#include "core/components/Shader.hpp"

Material3D::Material3D(rgr::Texture* texture)
{
	m_Texture = texture;
	m_Shader = rgr::Shader::FromFiles("resources/shaders/vertex_3d.glsl", "resources/shaders/fragment_3d.glsl");
}

void Material3D::SetUniforms()
{
	BindTexture("u_Texture", m_Texture, 0);
}

Material3D::~Material3D()
{

}
