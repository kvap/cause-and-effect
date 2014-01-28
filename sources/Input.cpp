#include "Input.hpp"

#include <GLFW/glfw3.h>

#define DEVICE_TYPE_COUNT 3
static bool enabledDevices[DEVICE_TYPE_COUNT];
const GLFWwindow* glfwWindow;

void Input::initialize(const GLFWwindow* window)
{
	glfwWindow = window;
	for (int i = 0; i < DEVICE_TYPE_COUNT; ++i)
		enabledDevices[i] = false;
}

void Input::loadShortcuts(std::string shortcutFile)
{
}

void Input::saveShortcuts(std::string shortcutFile)
{
}

void Input::enable(DeviceType deviceType)
{
	enabledDevices[deviceType] = true;
}

void Input::disable(DeviceType deviceType)
{
	enabledDevices[deviceType] = false;
}

const Shortcut* Input::getNewShortcut()
{
	
}

void Input::bindShortcut(int actionId, const Shortcut* shortcut)
{
}

bool Input::isPressed(int actionId)
{
	return false;
}

bool Input::isReleased(int actionId)
{
	return false;
}

bool Input::isFirstPressed(int actionId)
{
	return false;
}

bool Input::isFirstReleased(int actionId)
{
	return false;
}

void Input::updateEvents()
{

	glfwPollEvents();
}

void Input::terminate()
{

}
