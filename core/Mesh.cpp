#include <thirdparty/glad/glad.h>
#include <thirdparty/stb/stb_image.h>

#include "Mesh.h"

int Texture::loadTexture(char* path)
{
	glGenTextures(1, &id);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (!data)
	{
		std::cout << "CORE::TEXTURE::FAILED_TO_LOAD" << std::endl;
		stbi_image_free(data);
		return -1;
	}

	GLenum format;
	if (nrComponents >= 1 && nrComponents <= 4)
	{
		GLenum formats[4] = {GL_RED, GL_RG, GL_RGB, GL_RGBA};
		format = formats[nrComponents];
	}
	else
	{
		std::cout << "CORE::TEXTURE::NR_COMPONENTS_INVALID" << std::endl;
		stbi_image_free(data);
		return -1;
	}

	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_image_free(data);
	return id;
}