#pragma once

#include "gtc/matrix_transform.hpp"
#include "gtc/quaternion.hpp"
#include "glm.hpp"

namespace rgr
{
    class Transform
    {
    public:
        enum class Space
        {
            WORLD_3D, 
            SCREEN_2D
        };

        Space space = Space::WORLD_3D;

        Transform();
        Transform(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale);
        ~Transform();

        void SetPosition(const glm::vec3& position);
        void SetRotation(const glm::quat& rotation);
        void SetScale(const glm::vec3& scale);

        inline glm::vec3 GetPosition() const { return m_Position; }
        inline glm::quat GetRotation() const { return m_Rotation; }
        inline glm::vec3 GetScale() const { return m_Scale; }
        glm::vec3 GetForwardVector();
        glm::vec3 GetRightVector();

        glm::mat4& GetModelMatrix();
        glm::mat3& GetNormalMatrix();
    private:
        glm::vec3 m_Position;
        glm::quat m_Rotation;
        glm::vec3 m_Scale;

        mutable glm::mat4 m_ModelMatrix;
        mutable glm::mat3 m_NormalMatrix;

        bool m_ShouldUpdate = true;
    };
}

