#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <dirent.h>

#include "util/Logger.hpp"
#include "ResourceManager.h"
#include "Input.hpp"
#include "GameTime.h"
#include "ResourceManager.h"
 
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
	LOG_FATAL("GLFW Error(" + stringify<int>(error) + "): " + description);
}

enum State {
	STATE_ENTRANCE,
	STATE_LIST,
	STATE_SCENE,
	STATE_CONFIG,
	STATE_EXIT
};

State config(GLFWwindow *window, Font *font) {
	Camera c(Point(0, 0), Point(0, 0), Point(screen_width, screen_height), 1);
	Texture *logo = ResourceManager::getTexture("logo2");

	int actions[7] = {
		ACTION_OK,
		ACTION_LEFT, ACTION_RIGHT, ACTION_UP, ACTION_DOWN, ACTION_JUMP,
		ACTION_CANCEL
	};
	std::string names[7] = {
		"OK",
		"LEFT", "RIGHT", "UP", "DOWN", "JUMP",
		"CANCEL"
	};
	int curaction = 0;

	while (curaction < 7) {
		const Shortcut *shortcut = Input::getNewShortcut();
		if (shortcut) {
			Input::bindShortcut(actions[curaction], shortcut);
			LOG_STRING("Binded new " + names[curaction] + " shortcut.");
			curaction++;
		}

		c.apply_viewport();
		glTranslated(screen_width / 2, screen_height / 2, 0);

		glClearColor(0.3, 0.3, 0.3, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		logo->apply();
		glColor3f(1, 1, 1);
		glBegin(GL_QUADS);
			glTexCoord2d(0, 0); glVertex2d(-logo->getWidth() - 5, -logo->getHeight() / 2);
			glTexCoord2d(0, 1); glVertex2d(-logo->getWidth() - 5,  logo->getHeight() / 2);
			glTexCoord2d(1, 1); glVertex2d(                  - 5,  logo->getHeight() / 2);
			glTexCoord2d(1, 0); glVertex2d(                  - 5, -logo->getHeight() / 2);
		glEnd();

		double x = 0;
		double y = 1.5 * font->getLineHeight();
		for (int i = 0; i < 7; i++) {
			if (i == curaction) {
				glColor3f(0.5, 0.5, 1.0);
			} else {
				glColor3f(0.7, 0.7, 0.7);
			}
			font->printString("[ " + names[i] + " ]", x, y, 1, ALIGN_LEFT);
			y -= 1.5 * font->getLineHeight();
		}

		glfwSwapBuffers(window);
		Input::updateEvents();
	}
	Input::saveShortcuts("controls.cfg");
	return STATE_ENTRANCE;
}

State entrance(GLFWwindow *window, Font *font) {
	Camera c(Point(0, 0), Point(0, 0), Point(screen_width, screen_height), 1);
	Texture *logo = ResourceManager::getTexture("logo2");
	while (true) {
		if (Input::isJustPressed(ACTION_OK)) {
			return STATE_LIST;
		} else if (Input::isJustPressed(ACTION_CANCEL)) {
			return STATE_EXIT;
		} else {
			const Shortcut *shortcut = Input::getNewShortcut();
			if (shortcut) {
				delete shortcut;
				return STATE_CONFIG;
			}
		}

		c.apply_viewport();
		glTranslated(screen_width / 2, screen_height / 2, 0);

		glClearColor(0.3, 0.3, 0.3, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		logo->apply();
		glColor3f(1, 1, 1);
		glBegin(GL_QUADS);
			glTexCoord2d(0, 0); glVertex2d(-logo->getWidth() - 5, -logo->getHeight() / 2);
			glTexCoord2d(0, 1); glVertex2d(-logo->getWidth() - 5,  logo->getHeight() / 2);
			glTexCoord2d(1, 1); glVertex2d(                  - 5,  logo->getHeight() / 2);
			glTexCoord2d(1, 0); glVertex2d(                  - 5, -logo->getHeight() / 2);
		glEnd();

		glColor3f(0.7, 0.7, 0.7);
		double x = 0;
		double y = 1.5 * font->getLineHeight();
		font->printString("Press [ OK ] to continue", x, y, 1, ALIGN_LEFT);
		y -= 1.5 * font->getLineHeight();
		font->printString("Press [ CANCEL ] to exit", x, y, 1, ALIGN_LEFT);
		y -= 1.5 * font->getLineHeight();
		font->printString("Press anything else to reconfigure", x, y, 1, ALIGN_LEFT);

		glfwSwapBuffers(window);
		Input::updateEvents();
	}
	return STATE_EXIT;
}

// FIXME: find a crossplatfom way to do this
void list_dir(std::vector<std::string> *v) {
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir("scenes")) != NULL) {
		/* print all the files and directories within directory */
		while ((ent = readdir(dir)) != NULL) {
			std::string name(ent->d_name);
			if ((name != ".") && (name != "..") && (name.substr(name.size() - 4, name.size()) == ".svg")) {
				v->push_back(name.substr(0, name.size() - 4));
			}
		}
		closedir (dir);
	} else {
		/* could not open directory */
		LOG_ERROR("Could not open directory 'scenes'");
	}
	std::sort(v->begin(), v->end());
}

std::string selectedSceneName;

State list(GLFWwindow *window, Font *font) {
	Camera c(Point(0, 0), Point(0, 0), Point(screen_width, screen_height), 1);
	std::vector<std::string> scenes;
	list_dir(&scenes);

	int selected = 0;
	while (true) {
		if (Input::isJustPressed(ACTION_OK)) {
			selectedSceneName = scenes[selected];
			return STATE_SCENE;
		} else if (Input::isJustPressed(ACTION_CANCEL)) {
			return STATE_ENTRANCE;
		} else if (Input::isJustPressed(ACTION_UP)) {
			selected = (selected - 1 + scenes.size()) % scenes.size();
		} else if (Input::isJustPressed(ACTION_DOWN)) {
			selected = (selected + 1) % scenes.size();
		}

		c.apply_viewport();

		glClearColor(0.7, 0.7, 0.7, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		double x = screen_width / 2;
		double y = screen_height / 2 + 1.5 * selected * font->getLineHeight();
		for (int i = 0; i < scenes.size(); i++) {
			if (i == selected) {
				glColor3f(0.3, 0.3, 0.3);
				double wid = font->getStringWidth(scenes[i]);
				double margin = 10;
				glDisable(GL_TEXTURE_2D);
				glBegin(GL_QUADS);
					glVertex2d(x - margin, y - margin);
					glVertex2d(x + wid + margin, y - margin);
					glVertex2d(x + wid + margin, y + font->getLineHeight());
					glVertex2d(x - margin, y + font->getLineHeight());
				glEnd();
				glColor3f(0.7, 0.7, 0.7);
			} else {
				glColor3f(0.3, 0.3, 0.3);
			}
			font->printString(scenes[i], x, y, 1, ALIGN_LEFT);
			glColor3f(0.3, 0.3, 0.3);
			font->printString(stringify<int>(i) + ": ", x - 10, y, 1, ALIGN_RIGHT);
			y -= 1.5 * font->getLineHeight();
		}

		glfwSwapBuffers(window);
		Input::updateEvents();
	}
	return STATE_EXIT;
}

bool playScene(GLFWwindow *window, Font *font, std::string sceneName) {
	Camera c1(Point(0, 0), Point(0, screen_height/2), Point(screen_width, screen_height), 40);
	Camera c2(Point(0, 0), Point(0, 0), Point(screen_width, screen_height/2), 40);
	
	GameScene *scene = ResourceManager::getGameScene(sceneName);

	GameTime gameTime;
	gameTime.totalGameTime = 0.0;
	double startGameTime = glfwGetTime();

	double physicsTime = 0.0;
	while (true)
	{
		if (Input::isJustPressed(ACTION_CANCEL)) {
			return false;
		}

		double currentGameTime = glfwGetTime();
		#define MAX_ELAPSED 0.1
		gameTime.elapsedGameTime = currentGameTime - startGameTime - gameTime.totalGameTime;
		if (gameTime.elapsedGameTime > MAX_ELAPSED) {
			gameTime.elapsedGameTime = MAX_ELAPSED;
		}
		gameTime.totalGameTime += gameTime.elapsedGameTime;

		physicsTime += gameTime.elapsedGameTime;

		if (!scene->getScenePlayer(0) || !scene->getScenePlayer(1))
			LOG_FATAL("Scene player not found.")

		c1.setPos(scene->getScenePlayer(0)->getPosition());
		c2.setPos(scene->getScenePlayer(1)->getPosition());

		//if (scene->getScenePlayer(0)->getPosition().x > scene->getScenePlayer(1)->getPosition().x) {
		//	// FIXME: implement graceful level ending
		//	//ResourceManager::removeScene(sceneName);
		//	LOG_STRING("scene ended");
		//	return true;
		//}

		// update & draw scenes here.
		c1.apply();
		glClearColor(0.3, 0.3, 0.3, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		glColor3f(1, 1, 1);
		scene->draw(&gameTime);
		c1.apply_viewport();
		font->printString("Cause", screen_width / 2, screen_height / 6, 1, ALIGN_CENTER);
//		if (scene->getScenePlayer(0)->getPosition().x > scene->getScenePlayer(1)->getPosition().x) {
//			f->printString("победа!", screen_width / 2, screen_height / 4, 1, ALIGN_CENTER);
//		}


		c2.apply();
		glClearColor(0.7, 0.7, 0.7, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		glColor3f(0, 0, 0);
		scene->draw(&gameTime);
		c2.apply_viewport();
		font->printString("Effect", screen_width / 2, screen_height / 6, 1, ALIGN_CENTER);
//		if (scene->getScenePlayer(0)->getPosition().x > scene->getScenePlayer(1)->getPosition().x) {
//			f->printString("победа!", screen_width / 2, screen_height / 4, 1, ALIGN_CENTER);
//		}

		scene->update(&gameTime);
		while (physicsTime >= 1.0f/60.0f) {
			scene->updatePhysics();
			physicsTime -= 1.0f/60.0f;
		}

		glfwSwapBuffers(window);
		Input::updateEvents();
	}
	return false;
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

	screen_width = 640;
	screen_height = 480;
	GLFWwindow *window = setup_window(&screen_width, &screen_height, true, false);
	Input::initialize(window);
	Input::enable(Input::KEYBOARD);
	Input::loadShortcuts("controls.cfg");

	Sound::initialize();
	Sound* themeSound = ResourceManager::getSound("theme");
	themeSound->setLooping(true);
	themeSound->play();

	Font *font = Fonts::genFont("DroidSans.ttf", 20);
	State state = STATE_ENTRANCE;
	while (state != STATE_EXIT) {
		switch (state) {
			case STATE_ENTRANCE:
				state = entrance(window, font);
				break;
			case STATE_LIST:
				state = list(window, font);
				break;
			case STATE_SCENE:
				if (playScene(window, font, selectedSceneName)) {
					state = STATE_SCENE;
				} else {
					state = STATE_LIST;
				}
				break;
			case STATE_CONFIG:
				state = config(window, font);
				break;
			case STATE_EXIT:
				break;
		}
		LOG_STRING("state -> " + stringify<int>(state));
		Input::updateEvents();
	}
//	playScene(window, font, "test");

	Sound::terminate();
	Input::terminate();
	glfwDestroyWindow(window);
	glfwTerminate();
	LOG_CLOSE();
	return 0;
}
