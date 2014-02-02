#include "Input.hpp"
#include <stdio.h>
#include <map>
#include <fstream>
#include <GLFW/glfw3.h>

#define DEVICE_TYPE_COUNT 3
static bool enabledDevices[DEVICE_TYPE_COUNT];
GLFWwindow* glfwWindow;

#include "util/Logger.hpp"
#include "input/Keyboard.hpp"
#include "input/Mouse.hpp"
#include "input/Joystick.hpp"

typedef std::map<int, const Shortcut*> ActionMap;
static ActionMap actionMap;

void Input::initialize(GLFWwindow* window)
{
	glfwWindow = window;
	for (int i = 0; i < DEVICE_TYPE_COUNT; ++i)
		enabledDevices[i] = false;

	LOG_STRING("Input initialized.");
}

void Input::loadShortcuts(std::string shortcutFile)
{
	std::ifstream configFile;
	configFile.open(shortcutFile.c_str(), std::ios::in | std::ios::binary);
	if (!configFile.is_open())
	{
		LOG_ERROR("Can't open shortcut file to read.");
		return;
	}

	int actionCount = 0;
	configFile.read((char*)&actionCount, sizeof(int));

	for (int i = 0; i < actionCount; ++i)
	{
		int actionId = -1;
		configFile.read((char*)&actionId, sizeof(int));
		int shortcutCount = 0;
		configFile.read((char*)&shortcutCount, sizeof(int));
		for (int j = 0; j < shortcutCount; ++j)
		{
			int keyCount = 0;
			configFile.read((char*)&keyCount, sizeof(int));
			Key* shortcutKeys = new Key[keyCount];
			configFile.read((char*)shortcutKeys, sizeof(Key) * keyCount);

			Shortcut* shortcut = new Shortcut(shortcutKeys, shortcutKeys + keyCount);
			actionMap[actionId] = shortcut;
			LOG_STRING("Input: loaded new shortcut.");
		}
	}

	configFile.close();
}

void Input::saveShortcuts(std::string shortcutFile)
{
	std::ofstream configFile;
	configFile.open(shortcutFile.c_str(), std::ios::out | std::ios::binary);
	if (!configFile.is_open())
	{
		LOG_ERROR("Can't open shortcut file to write.");
		return;
	}
	LOG_STRING("Input: save actions:");
	int actionCount = actionMap.size();
	configFile.write((char*)&actionCount, sizeof(int)); // actionCount
	LOG_STRING("Input: wrote action count");
	for (ActionMap::iterator it = actionMap.begin(); it != actionMap.end(); ++it)
	{
		configFile.write((char*)&(it->first), sizeof(int)); // actionId
		LOG_STRING("Input: wrote action id");
		int shortcutCount = 1;
		configFile.write((char*)&shortcutCount, sizeof(int)); // shortcutCount
		LOG_STRING("Input: wrote shortcut count (1)");
		int keyCount = it->second->size();
		configFile.write((char*)&keyCount, sizeof(int)); // keyCount
		LOG_STRING("Input: wrote shortcut key count");
		Key* keys = new Key[keyCount];
		std::copy(it->second->begin(), it->second->end(), keys);
		configFile.write((char*)keys, sizeof(Key) * it->second->size()); // keys
		LOG_STRING("Input: wrote shortcut keys");
	}

	configFile.close();
}

void Input::enable(DeviceType deviceType)
{
	switch (deviceType)
	{
		case Input::KEYBOARD:
			glfwSetKeyCallback(glfwWindow, Keyboard::keyCallback);
			LOG_STRING("Input: Keyboard device is enabled");
			break;
		case Input::MOUSE:
			break;
		case Input::JOYSTICK:
			break;
		default: // Unknown device type.
			LOG_STRING("Input: Unknown device type");
			break;
	}
	
	enabledDevices[deviceType] = true;
}

void Input::disable(DeviceType deviceType)
{
	switch (deviceType)
	{
		case Input::KEYBOARD:
			glfwSetKeyCallback(glfwWindow, NULL);
			LOG_STRING("Input: Keyboard device is disabled");
			break;
		case Input::MOUSE:
			break;
		case Input::JOYSTICK:
			break;
		default: // Unknown device type.
			LOG_STRING("Input: Unknown device type");
			break;
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
	return !isPressed(actionId);
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
	ActionMap::iterator it = actionMap.find(actionId);
	if (it == actionMap.end()) {
		LOG_WARNING("Bad actionId");
		return false;
	}

	const Shortcut* actionShortcut = it->second;
	for (int i = 0; i < actionShortcut->size(); ++i)
	{
		if (actionShortcut[0][i].deviceType == Input::KEYBOARD)
		{
			if (Keyboard::keyIsReleased(actionShortcut[0][i].keyCode)
				&& !Keyboard::keyIsJustReleased(actionShortcut[0][i].keyCode))
				return false;
		}
	}

	return true;
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
