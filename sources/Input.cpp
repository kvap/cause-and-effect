#include "Input.hpp"
#include <stdio.h>
#include <map>
#include <GLFW/glfw3.h>

#define DEVICE_TYPE_COUNT 3
static bool enabledDevices[DEVICE_TYPE_COUNT];
GLFWwindow* glfwWindow;

#include "util/Logger.hpp"
#include "input/Keyboard.hpp"
#include "input/Mouse.hpp"
#include "input/Joystick.hpp"

void Input::initialize(GLFWwindow* window)
{
	glfwWindow = window;
	for (int i = 0; i < DEVICE_TYPE_COUNT; ++i)
		enabledDevices[i] = false;

	LOG_STRING("Input initialized.");
}

void Input::loadShortcuts(std::string shortcutFile)
{
}

void Input::saveShortcuts(std::string shortcutFile)
{
}

void Input::enable(DeviceType deviceType)
{
	if (deviceType == Input::KEYBOARD)
	{
		glfwSetKeyCallback(glfwWindow, Keyboard::keyCallback);
		LOG_STRING("Input: Keyboard device is enabled");
	}

	enabledDevices[deviceType] = true;
}

void Input::disable(DeviceType deviceType)
{
	if (deviceType == Input::KEYBOARD)
	{
		glfwSetKeyCallback(glfwWindow, NULL);
		LOG_STRING("Input: Keyboard device is disabled");
	}

	enabledDevices[deviceType] = false;
}

static Shortcut* genShortcut = new Shortcut();
static Shortcut* lastShortcut;
const Shortcut* Input::getNewShortcut()
{
	int keyCount = 0;
	Keyboard::getPressedKeys(keyCount);
	if (genShortcut->size() > 0 && keyCount == 0)
	{
		lastShortcut = genShortcut;
		genShortcut = new Shortcut();
		LOG_STRING("Input: Returned new shortcut.");
		return lastShortcut;
	}
	
	int* kbJustPressed = Keyboard::getJustPressedKeys(keyCount);
	for (int i = 0; i < keyCount; ++i)
	{
		Key key;
		key.deviceType = Input::KEYBOARD;
		key.keyCode = kbJustPressed[i];
		genShortcut->push_back(key);
		LOG_STRING("Input: Added new key into shortcut.");
	}
	
	return NULL;
}

typedef std::map<int, const Shortcut*> ActionMap;
static ActionMap actionMap;

void Input::bindShortcut(int actionId, const Shortcut* shortcut)
{
	actionMap[actionId] = shortcut;
	LOG_STRING("Input: binded new shortcut")
}

bool Input::isPressed(int actionId)
{
	ActionMap::iterator it = actionMap.find(actionId);
	if (it == actionMap.end())
	{
		LOG_WARNING("Bad actionId");
		return false;
	}
	
	const Shortcut* actionShortcut = it->second;
	for (int i = 0; i < actionShortcut->size(); ++i)
	{
		if (actionShortcut[0][i].deviceType == Input::KEYBOARD
			&& !Keyboard::keyIsPressed(actionShortcut[0][i].keyCode))
			return false;
	}

	return true;
}

bool Input::isReleased(int actionId)
{
	return false;
}

bool Input::isJustPressed(int actionId)
{
	ActionMap::iterator it = actionMap.find(actionId);
	if (it == actionMap.end()) {
		LOG_WARNING("Bad actionId");
		return false;
	}

	const Shortcut* actionShortcut = it->second;
	bool hasJustPressedKey = false;
	for (int i = 0; i < actionShortcut->size(); ++i)
	{
		if (actionShortcut[0][i].deviceType == Input::KEYBOARD)
		{
			if (!Keyboard::keyIsPressed(actionShortcut[0][i].keyCode))
				return false;
			if (Keyboard::keyIsJustPressed(actionShortcut[0][i].keyCode))
				hasJustPressedKey = true;
		}
	}

	return hasJustPressedKey;
}

bool Input::isJustReleased(int actionId)
{
	return false;
}

void Input::updateEvents()
{
	if (enabledDevices[Input::KEYBOARD])
		Keyboard::updateEvents();
	if (enabledDevices[Input::MOUSE])
		Mouse::updateEvents();
	if (enabledDevices[Input::JOYSTICK])
		Joystick::updateEvents();

	glfwPollEvents();
}

void Input::terminate()
{

}
