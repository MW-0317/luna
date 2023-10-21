#pragma once
#include <iostream>

#include <thirdparty/glad/glad.h>

#include "../Core.h"

#define STB_IMAGE_IMPLEMENTATION
#include <thirdparty/stb/stb_image.h>
#include <thirdparty/stb/stb_image_write.h>

class Image
{
private:
	int width, height, nrComponents;
	GLenum format;
	unsigned char* data;

public:
	// For getting image
	void init(const char* filename);
	Image(const char* filename);

	// For saving image
	void init(unsigned char* data, int width, int height, int nrComponents);
	Image(unsigned char* data, int width, int height, int nrComponents);

	void save(const char* filename);

	~Image();
};