#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "util/Logger.hpp"
#include "Keyboard.h"
#include "GameTime.h"
#include "GameScene.h"

#include "SceneLoader.h"
#include "GameObjects/Box.h"
#include "GameObjects/Character.h"

#include "graphics/Graphics.hpp"
#include "graphics/Camera.hpp"
#include "graphics/Textures.hpp"
#include "graphics/Fonts.hpp"
#include "graphics/Sprite.hpp"
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
	glfwSetKeyCallback(window, Keyboard::keyCallback);
	initTextures();

	Font *f = Fonts::genFont("DroidSans.ttf", 20);

	GameTime gameTime;
	gameTime.totalGameTime = 0.0;
	double startGameTime = glfwGetTime();

	//Character b1(Point(23, 110), Point(1, 1.5));
	//Box b2(Point(10, 50), Point(100, 10));
	//GameScene gs(1);
	//gs.layers[0]->add(&b1, Physics::DYNAMIC);
	//gs.layers[0]->add(&b2, Physics::STATIC);

	Camera c1(Point(0, 0), Point(0, screen_height/2), Point(screen_width, screen_height), 40);
	Camera c2(Point(0, 0), Point(0, 0), Point(screen_width, screen_height/2), 40);
	
	GameScene *scene = loadScene("test.svg");
	//Character ch(Point(23, 110), Point(1, 1.5));
	//scene->layers[0]->add(&ch, Physics::DYNAMIC);
	//LOG_FATAL("hello");

	double physicsTime = 0.0;
	while (!glfwWindowShouldClose(window))
	{
		double currentGameTime = glfwGetTime();
		gameTime.elapsedGameTime = currentGameTime - startGameTime - gameTime.totalGameTime;
		gameTime.totalGameTime += gameTime.elapsedGameTime;

		physicsTime += gameTime.elapsedGameTime;

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		if (!scene->getScenePlayer(0) || !scene->getScenePlayer(1))
			LOG_FATAL("Scene player not found.")
		c1.setPos(scene->getScenePlayer(0)->getPosition());
		c2.setPos(scene->getScenePlayer(1)->getPosition());

		f->printString("привет, мир!", 10, 20, 1, ALIGN_LEFT);

		// update & draw scenes here.
		c1.apply();
		scene->draw(&gameTime);
		c2.apply();
		scene->draw(&gameTime);
		scene->update(&gameTime);
		while (physicsTime >= 1.0f/60.0f) {
			scene->updatePhysics();
			physicsTime -= 1.0f/60.0f;
		}
		Keyboard::update();


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	LOG_CLOSE();
	return 0;
}
