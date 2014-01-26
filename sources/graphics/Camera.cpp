/*
 * ----------------------------------------------------------------------------
 * Constantin S. Pan <kvapen@gmail.com> wrote this file. As long as you retain
 * this notice you can do whatever you want with this stuff. If we meet some
 * day, and you think this stuff is worth it, you can buy me a can of Coke in
 * return.
 * 	Constantin S. Pan
 * ----------------------------------------------------------------------------
 */

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Camera.hpp"

#include "../Point.h"

Camera::Camera(Point pos, Point lower_left, Point upper_right, double zoom) {
	this->pos = pos;
	this->viewport_lower_left = lower_left;
	this->viewport_upper_right = upper_right;
	this->zoom = zoom;
}

// set all the necessary OpenGL transformations and the viewport
void Camera::apply() {
	int wid = viewport_upper_right.x - viewport_lower_left.x;
	int hei = viewport_upper_right.y - viewport_lower_left.y;
	glViewport(viewport_lower_left.x, viewport_lower_left.y, wid, hei);
	glScissor(viewport_lower_left.x, viewport_lower_left.y, wid, hei);
	glEnable(GL_SCISSOR_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-wid/2, wid/2, -hei/2, hei/2, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glScaled(zoom, zoom, 0);
	glTranslated(-pos.x, -pos.y, 0);
}

// set only viewport transformations (for UI)
void Camera::apply_viewport() {
	int wid = viewport_upper_right.x - viewport_lower_left.x;
	int hei = viewport_upper_right.y - viewport_lower_left.y;
	glViewport(viewport_lower_left.x, viewport_lower_left.y, wid, hei);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, wid, 0, hei, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Camera::setPos(double x, double y) {
	pos.x = x;
	pos.y = y;
}

void Camera::setPos(Point newpos) {
	pos = newpos;
}

void Camera::setZoom(double newzoom) {
	zoom = newzoom;
}
