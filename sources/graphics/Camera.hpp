/*
 * ----------------------------------------------------------------------------
 * Constantin S. Pan <kvapen@gmail.com> wrote this file. As long as you retain
 * this notice you can do whatever you want with this stuff. If we meet some
 * day, and you think this stuff is worth it, you can buy me a can of Coke in
 * return.
 * 	Constantin S. Pan
 * ----------------------------------------------------------------------------
 */

#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "../Point.h"

class Camera {
	private:
		Point pos;
		Point viewport_lower_left;
		Point viewport_upper_right;
		double zoom;
	public:
		Camera(Point pos, Point lower_left, Point upper_right, double zoom);

		// set all the necessary OpenGL transformations and the viewport
		void apply();

		// set only viewport transformations (for UI)
		void apply_viewport();

		void setPos(double x, double y);
		void setPos(Point newpos);

		void setZoom(double newzoom);
};

#endif

