#include "Transform.hpp"

namespace rgr
{
	Transform::Transform() : m_Position(0.0f, 0.0f, 1.0f), m_Rotation(1.0f, 0.0f, 0.0f, 0.0f), m_Scale(1.0f)
	{
	}

	Transform::Transform(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale)
	{	
		m_Scale = scale;
		if (space == Space::SCREEN_2D)
		{
			m_Rotation = glm::quat(glm::vec3(0.0f, 0.0f, glm::roll(rotation)));
			m_Position = glm::vec3(position.x, position.y, 1.0f);
		}
		else
		{
			m_Rotation = rotation;
			m_Position = position;
		}
	}

	Transform::~Transform() = default;

	void Transform::SetPosition(const glm::vec3& position)
	{
		m_Position = space == Space::SCREEN_2D ? glm::vec3(position.x, position.y, 1.0f) : position;
		m_ShouldUpdate = true;
	}
	void Transform::SetRotation(const glm::quat& rotation)
	{
		m_Rotation = space == Space::SCREEN_2D ? glm::quat(glm::vec3(0.0f, 0.0f, glm::roll(rotation))) : rotation;
		m_ShouldUpdate = true;
	}
	void Transform::SetScale(const glm::vec3& scale)
	{
		m_Scale = scale;
		m_ShouldUpdate = true;
	}

	glm::vec3 Transform::GetForwardVector()
	{
		constexpr auto forward = glm::vec3(0.0f, 0.0f, -1.0f);
		const glm::vec4 res = GetModelMatrix() * glm::vec4(forward, 0.0f);
		return glm::normalize(glm::vec3(res));
	}

	glm::vec3 Transform::GetRightVector()
	{
        constexpr auto right = glm::vec3(1.0f, 0.0f, 0.0f);
		const glm::vec4 res = GetModelMatrix() * glm::vec4(right, 0.0f);
		return glm::normalize(glm::vec3(res));
	}

    glm::vec3 Transform::GetUpVector()
    {
        constexpr auto up = glm::vec3(0.0f, 1.0f, 0.0f);
        const glm::vec4 res = GetModelMatrix() * glm::vec4(up, 0.0f);
        return res;
    }

	glm::mat4& Transform::GetModelMatrix()
	{
		if (m_ShouldUpdate)
		{
			m_ModelMatrix = glm::mat4(1.0f);

			m_ModelMatrix = glm::translate(m_ModelMatrix, m_Position);
			m_ModelMatrix *= glm::mat4_cast(m_Rotation);
			m_ModelMatrix = glm::scale(m_ModelMatrix, m_Scale);

			m_NormalMatrix = glm::mat3(glm::transpose(glm::inverse(m_ModelMatrix)));
		}

		m_ShouldUpdate = false;
		return m_ModelMatrix;
	}
	glm::mat3& Transform::GetNormalMatrix()
	{
		return m_NormalMatrix;
	}
}
