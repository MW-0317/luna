#include "Image.h"

void Image::init(const char* filename)
{
	stbi_set_flip_vertically_on_load(true);

	data = stbi_load(filename, &width, &height, &nrComponents, 0);
	if (!data)
	{
		std::cout << "CORE::IMAGE::FAILED_TO_LOAD" << std::endl;
		std::cout << "\t" << filename << std::endl;
		stbi_image_free(data);
		return;
	}

	if (nrComponents >= 1 && nrComponents <= 4)
	{
		GLenum formats[4] = { GL_RED, GL_RG, GL_RGB, GL_RGBA };
		format = formats[nrComponents - 1];
	}
	else
	{
		std::cout << "CORE::IMAGE::NR_COMPONENTS_INVALID" << std::endl;
		stbi_image_free(data);
		return;
	}
}

Image::Image(const char* filename)
{
	init(filename);
}

void Image::init(unsigned char* data, int width, int height, int nrComponents)
{
	this->data = data;
	this->width = width;
	this->height = height;
	this->nrComponents = nrComponents;
}

Image::Image(unsigned char* data, int width, int height, int nrComponents)
{
	init(data, width, height, nrComponents);
}

void Image::save(const char* filename)
{
	stbi_write_png(filename, width, height, nrComponents, data, nrComponents * width);
}

Image::~Image()
{
	stbi_image_free(this->data);
}