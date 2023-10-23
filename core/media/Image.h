#pragma once
#include <iostream>

#include <thirdparty/glad/glad.h>

#include "../Core.h"

namespace luna
{
	class LUNA_API Image
	{
	public:
		int width, height, nrComponents;
		GLenum format;
		unsigned char* data;

		// For getting image
		void init(const char* filename, bool flip = true);
		Image(const char* filename, bool flip = true);

		// For saving image
		void init(unsigned char* data, int width, int height, int nrComponents);
		Image(unsigned char* data, int width, int height, int nrComponents);

		void save(const char* filename);

		~Image();
	};
}