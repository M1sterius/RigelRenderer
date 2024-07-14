#pragma once

#include "gtc/matrix_transform.hpp"
#include "objects/Object.hpp"
#include "gtc/quaternion.hpp"
#include "mat4x4.hpp"
#include "glm.hpp"

namespace rgr
{
	class Camera : public rgr::Object
	{
	public:
		enum class ViewMode
		{
			FILL,
			WIREFRAME
		};

		ViewMode viewMode = ViewMode::FILL;
		float shadowsVisibilityDistance = 30.0f;

		Camera(float fov, float width, float height, float near, float far);
		~Camera() override = default;

		void FlagAsMain();
		void UnflagAsMain();
		inline bool IsMain() const { return m_IsMain; }

		glm::mat4& GetView();
		glm::vec3 GetForwardVector();
		inline glm::mat4& GetPerspective() const { return m_Perspective; }
		inline glm::mat4& GetOrthographic() const { return m_Orthographic; }
	private:
		bool m_IsMain;

		float m_Fov;
		float m_Width;
		float m_Height;
		float m_Near;
		float m_Far;

		mutable glm::mat4 m_Perspective;
		mutable glm::mat4 m_Orthographic;
		mutable glm::mat4 m_View;

		void ConstructProjectionMatrices();
	};
}

