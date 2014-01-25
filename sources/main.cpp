#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "util/Logger.hpp"
#include "Keyboard.h"
#include "GameTime.h"
#include "GameScene.h"

#include "GameObjects/Box.h"

#include "graphics/Graphics.hpp"
#include "graphics/Camera.hpp"
#include "graphics/Textures.hpp"
#include "graphics/Fonts.hpp"
#include "util/Logger.hpp"
#include "util/convert.hpp"

int screen_width, screen_height;

void errorCallback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error(%d): %s\n", error, description);
}

int main(int argc, char** argv)
{
	LOG_OPEN("log.txt");
	glfwSetErrorCallback(errorCallback);

	if (glfwInit() != GL_TRUE) {
		LOG_FATAL("GLFW initializing failed");
	}
	int glfw_version[3];
	glfwGetVersion(&glfw_version[0], &glfw_version[1], &glfw_version[2]);
	LOG_STRING("GLFW version "
			+ stringify<int>(glfw_version[0]) + "."
			+ stringify<int>(glfw_version[1]) + "."
			+ stringify<int>(glfw_version[2]) + " initialized"
		  );

	GLFWwindow *window = setup_window(&screen_width, &screen_height, true, true);
	initTextures();

	Font *f = Fonts::genFont("DroidSans.ttf", 20);

	GameTime gameTime;
	gameTime.totalGameTime = 0.0;
	double startGameTime = glfwGetTime();

	Box b1(Point(23, 10), Point(10, 10)), b2(Point(10, 50), Point(100, 10));
	GameScene gs(1);
	gs.layers[0]->add(&b1, Physics::DYNAMIC);
	gs.layers[0]->add(&b2, Physics::STATIC);

	Camera c(Point(0, 0), Point(10, 10), Point(1300, 600), 2);

	double physicsTime = 0.0;
	while (!glfwWindowShouldClose(window))
	{
		double currentGameTime = glfwGetTime();
		gameTime.elapsedGameTime = currentGameTime - startGameTime - gameTime.totalGameTime;
		gameTime.totalGameTime += gameTime.elapsedGameTime;

		physicsTime += gameTime.elapsedGameTime;

		c.apply();
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// update & draw scenes here.
		gs.draw(&gameTime);
		gs.update(&gameTime);
		while (physicsTime >= 1.0f/60.0f) {
			gs.updatePhysics();
			physicsTime -= 1.0f/60.0f;
		}
		Keyboard::update();

		c.apply_viewport();
		int wid, hei;
		glEnable(GL_TEXTURE_2D);
		int tex = loadTexture("tux.png", &wid, &hei);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBindTexture(GL_TEXTURE_2D, tex);
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex2f(0, 0);
		glTexCoord2f(0, 1); glVertex2f(0, hei);
		glTexCoord2f(1, 1); glVertex2f(wid, hei);
		glTexCoord2f(1, 0); glVertex2f(wid, 0);
		glEnd();

		f->printString("привет, мир!", 10, 20, 1, ALIGN_LEFT);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	LOG_CLOSE();
	return 0;
}
