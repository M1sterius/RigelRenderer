#include "Cursor.hpp"
#include "Core.hpp"
#include "glfw3.h"

namespace rgr
{
    Cursor::CURSOR_STATE Cursor::m_CursorState = Cursor::CURSOR_STATE::NORMAL;

    void Cursor::SetCursorState(const CURSOR_STATE state)
    {
        auto windowPtr = rgr::Core::GetWindowPtr();
        m_CursorState = state;

        switch (state)
        {
            case CURSOR_STATE::NORMAL:
                glfwSetInputMode(windowPtr, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                break;
            case CURSOR_STATE::HIDDEN:
                glfwSetInputMode(windowPtr, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
                break;
            case CURSOR_STATE::LOCKED:
                glfwSetInputMode(windowPtr, GLFW_CURSOR, GLFW_CURSOR_CAPTURED);
                break;
            case CURSOR_STATE::DISABLED:
                glfwSetInputMode(windowPtr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                break;
        }
    }

    Cursor::CURSOR_STATE Cursor::GetCursorState()
    {
        return m_CursorState;
    }
}
