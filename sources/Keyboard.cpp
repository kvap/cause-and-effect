#include "Keyboard.h"
#include <cstdio>

static int keys[GLFW_KEY_LAST + 1];

Keyboard::Keyboard()
{ }

void Keyboard::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
//	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
//		glfwSetWindowShouldClose(window, GL_TRUE);
//    if (action == GLFW_PRESS) {
//        printf("key callback: %d -> press\n", key);
//    } else {
//        printf("key callback: %d -> repeat\n", key);
//    }
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

void Keyboard:: updateEvents()
{
    for (int i = 0; i <= GLFW_KEY_LAST; ++i)
        if (keys[i] == GLFW_PRESS)
            keys[i] = GLFW_REPEAT;
}
