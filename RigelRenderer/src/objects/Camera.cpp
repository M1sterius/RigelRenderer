#include "Camera.hpp"

#include <iostream>
#include "gtx/string_cast.hpp"

namespace rgr
{
	Camera::Camera(float fov, float width, float height, float near, float far)
	{
		m_Fov = fov;
		m_Width = width;
		m_Height = height;
		m_Near = near;
		m_Far = far;

		ConstructProjectionMatrices();
	}
	Camera::~Camera()
	{
		
	}
	void Camera::FlagAsMain()
	{
		m_IsMain = true;
	}
	void Camera::UnflagAsMain()
	{
		m_IsMain = false;
	}
	glm::mat4& Camera::GetView()
	{	
		glm::vec3 pos = GetTransform().GetPosition();
		glm::quat rot = GetTransform().GetRotation();
		glm::mat4 transformation = glm::translate(glm::mat4(1.0f), pos) * glm::mat4_cast(rot);
		m_View = glm::inverse(transformation);

		return m_View;
	}
	glm::vec3 Camera::GetForwardVector()
	{
		glm::mat4 mat = GetTransform().GetModelMatrix();
		return glm::normalize(glm::vec3(mat[2])) * glm::vec3(-1.0f);
	}
	void Camera::ConstructProjectionMatrices()
	{
		m_Perspective = glm::perspective(m_Fov, m_Width / m_Height, m_Near, m_Far);
		m_Orthographic = glm::ortho(-m_Width / 2, m_Width / 2, -m_Height / 2, m_Height / 2, -1.0f, 1.0f);
	}
}