#include "Keyboard.h"
#include <cstdio>

static int keys[GLFW_KEY_LAST + 1];

Keyboard::Keyboard()
{ }

void Keyboard::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) {
        printf("key callback: %d -> press\n", key);
    } else {
        printf("key callback: %d -> repeat\n", key);
    }
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

