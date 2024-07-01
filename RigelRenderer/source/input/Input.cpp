#include "Input.hpp"

#include "utility/InputUtility.hpp"

namespace rgr
{
	glm::vec2 Input::GetMousePos()
	{
		return InputUtility::mousePos;
	}

	glm::vec2 Input::GetMouseDelta()
	{
		return InputUtility::mouseDelta;
	}

	bool Input::KeyPressed(int key)
	{
		return InputUtility::InKeys(key) && !InputUtility::InOldKeys(key);
	}
	bool Input::KeyHold(int key)
	{
		return InputUtility::InKeys(key);
	}
	bool Input::KeyReleased(int key)
	{
		return !InputUtility::InKeys(key) && InputUtility::InOldKeys(key);
	}
	bool Input::InKeys(int key)
	{
		return InputUtility::InKeys(key);
	}
	bool Input::InOldKeys(int key)
	{
		return InputUtility::InOldKeys(key);
	}
}