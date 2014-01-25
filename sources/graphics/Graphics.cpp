/*
 * ----------------------------------------------------------------------------
 * Constantin S. Pan <kvapen@gmail.com> wrote this file. As long as you retain
 * this notice you can do whatever you want with this stuff. If we meet some
 * day, and you think this stuff is worth it, you can buy me a can of Coke in
 * return.
 * 	Constantin S. Pan
 * ----------------------------------------------------------------------------
 */

#include "Graphics.hpp"

#include "../util/Logger.hpp"
#include "../util/convert.hpp"

GLFWwindow *setup_window(int *resx, int *resy, bool vsync, bool fullscreen) {
	const GLFWvidmode *desktop = glfwGetVideoMode(glfwGetPrimaryMonitor());
	*resx = desktop->width;
	*resy = desktop->height;

	glfwWindowHint(GLFW_RED_BITS, desktop->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, desktop->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, desktop->blueBits);
	glfwWindowHint(GLFW_DEPTH_BITS, 32);
	glfwWindowHint(GLFW_STENCIL_BITS, 0);

	GLFWwindow *window = glfwCreateWindow(
		*resx, *resy,
		"Cause and Effect",
		fullscreen ? glfwGetPrimaryMonitor() : NULL,
		NULL
	);
	if (window) {
		LOG_STRING("Window opened");
	} else {
		LOG_FATAL("Could not create the window");
	}

	glfwMakeContextCurrent(window);
	LOG_STRING(std::string("OpenGL version: ") + (char*)glGetString(GL_VERSION));
	LOG_STRING(std::string("GLSL version: ") + (char*)glGetString(GL_SHADING_LANGUAGE_VERSION));

	int glew_status = glewInit();
	if (glew_status == GLEW_OK) {
		LOG_STRING(std::string("GLEW version: ") + (char*)glewGetString(GLEW_VERSION));
	} else {
		LOG_FATAL(std::string("GLEW init failed: ") + (char*)glewGetErrorString(glew_status));
	}

	int depth_bits;
	glGetIntegerv(GL_DEPTH_BITS, &depth_bits);
	LOG_STRING(std::string("Depth buffer bits = ") + stringify<int>(depth_bits));

	if (vsync) {
		glfwSwapInterval(1);
		LOG_STRING("vsync enabled");
	} else {
		glfwSwapInterval(0);
		LOG_STRING("vsync disabled");
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return window;
}

