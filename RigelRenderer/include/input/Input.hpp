#pragma once

#include "Keys.hpp"
#include "glm.hpp"

#include <unordered_set>

namespace rgr
{	
	class Input
	{
	public:
		static glm::vec2 GetMousePos();
		static glm::vec2 GetMouseDelta();

		// True only in the frame in which the key was pressed
		static bool KeyPressed(int key);
		// True during each frame the key is pressed
		static bool KeyHold(int key);
		// True only in the frame in which the key was released
		static bool KeyReleased(int key);

		// True if the given key is pressed during the current frame
		static bool InKeys(int key);
		// True if the given key was pressed during the previous frame
		static bool InOldKeys(int key);
	};
}

