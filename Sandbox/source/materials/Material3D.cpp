#include "Material3D.hpp"

#include "components/Shader.hpp"

Material3D::Material3D(rgr::Texture* texture, rgr::Shader* shader) : rgr::Material(shader)
{
	m_Texture = texture;
	m_Diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	m_Shininess = 16.0f;
}

void Material3D::SetUniforms()
{
	BindTexture("u_Texture", m_Texture, 0);
	SetUniformVec3("u_Material.diffuse", m_Diffuse);
	SetUniform1f("u_Material.shininess", m_Shininess);
}

Material3D::~Material3D()
{

}
