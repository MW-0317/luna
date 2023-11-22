#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_RESIZE2_IMPLEMENTATION
#define __STDC_LIB_EXT1__
#include <thirdparty/stb/stb_image.h>
#include <thirdparty/stb/stb_image_write.h>
#include <thirdparty/stb/stb_image_resize2.h>

namespace luna
{
	void Image::init(const char* filename, bool flip)
	{
		stbi_set_flip_vertically_on_load(flip);

		this->data = stbi_load(filename, &width, &height, &nrComponents, 0);
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
			stbir_pixel_layout layouts[4] =
			{
				stbir_pixel_layout::STBIR_1CHANNEL,
				stbir_pixel_layout::STBIR_RA,
				stbir_pixel_layout::STBIR_RGB,
				stbir_pixel_layout::STBIR_RGBA
			};
			format = formats[nrComponents - 1];
			pixFormat = layouts[nrComponents - 1];
		}
		else
		{
			std::cout << "CORE::IMAGE::NR_COMPONENTS_INVALID" << std::endl;
			stbi_image_free(data);
			return;
		}
	}

	Image::Image(const char* filename, bool flip)
	{
		init(filename, flip);
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

	void Image::resize(int width, int height)
	{
		unsigned char* newData = (unsigned char*) malloc(
			sizeof(unsigned char) * width * height * nrComponents
		);
		stbir_datatype      datatype = stbir_datatype::STBIR_TYPE_UINT8;
		stbir_edge			    edge = stbir_edge::STBIR_EDGE_CLAMP;
		stbir_filter          filter = stbir_filter::STBIR_FILTER_DEFAULT;

		stbir_resize(
			data, this->width, this->height, nrComponents * this->width, 
			newData, width, height, nrComponents * width,
			static_cast<stbir_pixel_layout>(pixFormat), datatype, edge, filter
		);

		if (!newData)
		{
			std::cout << "CORE::IMAGE::FAILED_TO_RESIZE" << std::endl;
			stbi_image_free(data);
			return;
		}
		data = newData;
		this->width = width;
		this->height = height;
	}

	Image::~Image()
	{
		stbi_image_free(this->data);
	}
}