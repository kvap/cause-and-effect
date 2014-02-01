#ifndef INPUT_HPP
#define INPUT_HPP
class Input;

#define JUMP 1

#include <string>
#include <GLFW/glfw3.h>
#include "input/Shortcut.hpp"

class Input
{
private:
	Input() { }
	~Input() { }
public:
	enum DeviceType
	{
		KEYBOARD = 0,
		MOUSE = 1,
		JOYSTICK = 2
	};

	static void initialize(GLFWwindow* window);

	static void loadShortcuts(std::string shortcutFile);
	static void saveShortcuts(std::string shortcutFile);

	static void enable(DeviceType deviceType);
	static void disable(DeviceType deviceType);

	static const Shortcut* getNewShortcut();
	static void bindShortcut(int actionId, const Shortcut* shortcut);

	static bool isPressed(int actionId);
	static bool isReleased(int actionId);
	static bool isJustPressed(int actionId);
	static bool isJustReleased(int actionId);

	static void updateEvents();

	static void terminate();
};

#endif // INPUT_HPP
