#include "InputUtility.hpp"

namespace rgr
{	
	std::unordered_set<int> InputUtility::keys = { };
	std::unordered_set<int> InputUtility::oldKeys = { };
	glm::vec2 InputUtility::mousePos = glm::vec2(0.0f);
	glm::vec2 InputUtility::oldMousePos = glm::vec2(0.0f);
	glm::vec2 InputUtility::mouseDelta = glm::vec2(0.0f);

	bool InputUtility::InKeys(int key)
	{
		return keys.find(key) != keys.end();
	}
	bool InputUtility::InOldKeys(int key)
	{
		return oldKeys.find(key) != oldKeys.end();
	}
}