#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <GLFW/glfw3.h>

class Keyboard
{
private:
    Keyboard();
protected:
//    static int keys[GLFW_KEY_LAST + 1];
public:
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    static bool keyIsFirstPressed(int key);
    static bool keyIsPressed(int key);

    static void updateEvents();
};

#endif // KEYBOARD_H
