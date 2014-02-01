#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include <GLFW/glfw3.h>

class Keyboard
{
	private:
		Keyboard() { }
		~Keyboard() { }
	public:
		static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

		static bool keyIsJustPressed(int key);
		static bool keyIsPressed(int key);
		static bool keyIsJustReleased(int key);
		static bool keyIsReleased(int key);

		static int* getJustPressedKeys(int &justPressedKeyCount);
		static int* getPressedKeys(int &pressedKeyCount);
		static int* getJustReleasedKeys(int &justReleasedKeyCount);

		static void updateEvents();
};

#endif // KEYBOARD_H
