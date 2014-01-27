/*
 * ----------------------------------------------------------------------------
 * Constantin S. Pan <kvapen@gmail.com> wrote this file. As long as you retain
 * this notice you can do whatever you want with this stuff. If we meet some
 * day, and you think this stuff is worth it, you can buy me a can of Coke in
 * return.
 * 	Constantin S. Pan
 * ----------------------------------------------------------------------------
 */

#include <map>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <cstring>
#include <png.h>
#include <malloc.h>

#include "../util/convert.hpp"
#include "../util/Logger.hpp"
#include "Textures.hpp"


int Texture::getWidth() {
	return width;
}

int Texture::getHeight() {
	return height;
}

unsigned int Texture::getHandle() {
	LOG_WARNING("try to not use this method, use 'apply' instead to bind the texture");
	return handle;
}

void Texture::apply() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, handle);
}

Texture::Texture(std::string filename) {
	// check if this file is already loaded
	LOG_STRING("Loading texture from file '" + filename + "'");
	handle = width = height = 0;

	FILE *f = fopen(filename.c_str(), "rb");
	if (!f) {
		LOG_ERROR("Cannot open file '" + filename + "'");
	}

	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr) {
		LOG_ERROR("png_create_read_struct() failed");
		png_destroy_read_struct(&png_ptr, NULL, NULL);
	}

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		LOG_ERROR("png_create_info_struct() failed");
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
	}

	if (setjmp(png_jmpbuf(png_ptr))) {
		LOG_ERROR("Error during reading PNG");
	}

	png_init_io(png_ptr, f);
	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_GRAY_TO_RGB | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_STRIP_16, NULL);

	unsigned char **image_rows = (unsigned char **)png_get_rows(png_ptr, info_ptr);

	int bytes_per_row = png_get_rowbytes(png_ptr, info_ptr);
	int color_type = png_get_color_type(png_ptr, info_ptr);
	width = png_get_image_width(png_ptr, info_ptr);
	height = png_get_image_height(png_ptr, info_ptr);

	unsigned char *image_data = (unsigned char *)malloc(bytes_per_row * height);
	for (int i = 0; i < height; i++) {
		memcpy(image_data + i * bytes_per_row, image_rows[height - i - 1], bytes_per_row);
	}

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &handle);
	glBindTexture(GL_TEXTURE_2D, handle);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	float maxAniso = 0;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);
	LOG_STRING("OpenGL max anisotropy: " + stringify<float>(maxAniso));
	if (maxAniso > 4) {
		maxAniso = 4;
	}
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAniso);

	// FIXME: fix the segfault and return back the mipmap generating
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	switch (color_type) {
		case PNG_COLOR_TYPE_RGB:
			LOG_STRING("PNG color type = RGB");
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
			//glGenerateMipmap(GL_TEXTURE_2D);
			break;
		case PNG_COLOR_TYPE_RGBA:
			LOG_STRING("PNG color type = RGBA");
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
			//glGenerateMipmap(GL_TEXTURE_2D);
			break;
		default:
			LOG_ERROR("Unsupported color type in the PNG");
	}

	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
	free(image_data);
	fclose(f);
}

Texture::~Texture() {
	glDeleteTextures(1,&handle);
}
