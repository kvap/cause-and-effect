#include "Keyboard.hpp"

#include <vector>
#include <algorithm>

static std::vector<int> justPressedKeys;
static std::vector<int> pressedKeys;
static std::vector<int> justReleasedKeys;

void Keyboard::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_UNKNOWN)
		return;

	switch (action)
	{
		case GLFW_PRESS:
			justPressedKeys.push_back(key);
			pressedKeys.push_back(key);
			break;
		case GLFW_RELEASE:
			pressedKeys.erase(
					std::remove(pressedKeys.begin(), pressedKeys.end(), key), pressedKeys.end()
				);
			justReleasedKeys.push_back(key);
			break;
		case GLFW_REPEAT:
			break;
		default: // Unknown action.
			break;
	}
}

bool Keyboard::keyIsJustPressed(int key)
{
	return std::find(justPressedKeys.begin(), justPressedKeys.end(), key) != justPressedKeys.end();
}

bool Keyboard::keyIsPressed(int key)
{
	return std::find(pressedKeys.begin(), pressedKeys.end(), key) != pressedKeys.end();
}

bool Keyboard::keyIsJustReleased(int key)
{
	return std::find(justReleasedKeys.begin(), justReleasedKeys.end(), key) != justReleasedKeys.end();
}

bool Keyboard::keyIsReleased(int key)
{
	return !keyIsPressed(key);
}

int* Keyboard::getJustPressedKeys(int &justPressedKeyCount)
{
	justPressedKeyCount = justPressedKeys.size();
	return &justPressedKeys[0];
}

int* Keyboard::getPressedKeys(int &pressedKeyCount)
{
	pressedKeyCount = pressedKeys.size();
	return &pressedKeys[0];
}

int* Keyboard::getJustReleasedKeys(int &justReleasedKeyCount)
{
	justReleasedKeyCount = justReleasedKeys.size();
	return &justReleasedKeys[0];
}

void Keyboard::updateEvents()
{
	justPressedKeys.clear();
	justReleasedKeys.clear();
}
