#include "Keyboard.h"

static int keys[GLFW_KEY_LAST + 1];

Keyboard::Keyboard()
{ }

void Keyboard::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key >= 0)
        keys[key] = action;
}

bool Keyboard::keyIsFirstPressed(int key) 
{
    return keys[key] == GLFW_PRESS;
}

bool Keyboard::keyIsPressed(int key) 
{
    return keys[key] == GLFW_PRESS || keys[key] == GLFW_REPEAT;
}

void Keyboard:: update()
{
    for (int i = 0; i <= GLFW_KEY_LAST; ++i)
        if (keys[i] == GLFW_PRESS)
            keys[i] = GLFW_REPEAT;
}
