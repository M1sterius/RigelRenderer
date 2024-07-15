#include "Input.hpp"

namespace rgr
{
    std::unordered_set<int> Input::keys = { };
    std::unordered_set<int> Input::oldKeys = { };
    glm::vec2 Input::mousePos = glm::vec2(0.0f);
    glm::vec2 Input::oldMousePos = glm::vec2(0.0f);
    glm::vec2 Input::mouseDelta = glm::vec2(0.0f);

    bool Input::InKeys(int key)
    {
        return keys.find(key) != keys.end();
    }

    bool Input::InOldKeys(int key)
    {
        return oldKeys.find(key) != oldKeys.end();
    }

	glm::vec2 Input::GetMousePos()
	{
		return mousePos;
	}

	glm::vec2 Input::GetMouseDelta()
	{
		return mouseDelta;
	}

	bool Input::KeyPressed(int key)
	{
		return InKeys(key) && !InOldKeys(key);
	}

	bool Input::KeyHold(int key)
	{
		return InKeys(key);
	}

	bool Input::KeyReleased(int key)
	{
		return !InKeys(key) && InOldKeys(key);
	}
}