#include <stdio.h>
#include <GLFW/glfw3.h>

#define SCREEN_WIDTH    640
#define SCREEN_HEIGHT   480

#include "Keyboard.h"
#include "GameTime.h"
#include "GameScene.h"

#include "GameObjects/Box.h"

void errorCallback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error(%d): %s\n", error, description);
}

void initOpengl()
{
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, 0.0, -1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char** argv)
{
    glfwSetErrorCallback(errorCallback);
    
    if (!glfwInit())
        return -1;
    
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Cause and Effect", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, Keyboard::keyCallback);
    
    initOpengl();

    GameTime gameTime;
    gameTime.totalGameTime = 0.0;
    double startGameTime = glfwGetTime();

    Box b1(Point(23, 10), Point(10, 10)), b2(Point(10, 50), Point(100, 10));
    GameScene gs(1);
    gs.layers[0]->add(&b1, Physics::DYNAMIC);
    gs.layers[0]->add(&b2, Physics::STATIC);

    double physicsTime = 0.0;
    while (!glfwWindowShouldClose(window))
    {
        double currentGameTime = glfwGetTime();
        gameTime.elapsedGameTime = currentGameTime - startGameTime - gameTime.totalGameTime;
        gameTime.totalGameTime += gameTime.elapsedGameTime;

        physicsTime += gameTime.elapsedGameTime;

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // update & draw scenes here.
        gs.draw(&gameTime);
        gs.update(&gameTime);
        while (physicsTime >= 1.0f/60.0f) {
            gs.updatePhysics();
            physicsTime -= 1.0f/60.0f;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
