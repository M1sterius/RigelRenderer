#pragma once

#include "glm.hpp"

#include <unordered_set>

namespace rgr
{
	class InputUtility
	{
	public:
		static std::unordered_set<int> keys;
		static std::unordered_set<int> oldKeys;
		static glm::vec2 mousePos;
		static glm::vec2 oldMousePos;
		static glm::vec2 mouseDelta;

		static bool InKeys(int key);
		static bool InOldKeys(int key);
	};
}

