#pragma once

#include "Keys.hpp"
#include "glm.hpp"
#include "Internal.hpp"

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
    INTERNAL:
        static std::unordered_set<int> keys;
        static std::unordered_set<int> oldKeys;
        static glm::vec2 mousePos;
        static glm::vec2 oldMousePos;
        static glm::vec2 mouseDelta;

        static bool InKeys(int key);
        static bool InOldKeys(int key);
	};
}

