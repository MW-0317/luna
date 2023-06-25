#include <thirdparty/glad/glad.h>
#include <thirdparty/stb/stb_image.h>

#include "Object.h"

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

std::vector<unsigned int> Mesh::triangulateMesh(std::vector<Vertex> vertices)
{
	struct Triangle
	{
		glm::vec3 indices;
	};

	std::vector<Triangle> triangulation = {};
	Triangle superTriangle;
	superTriangle.indices = glm::vec3(-1.0f, -1.0f, -1.0f);
	triangulation.push_back(superTriangle);
	
	for (int i = 0; i < vertices.size(); i++)
	{
		Vertex ver = vertices[i];

		std::vector<Triangle> badTriangles = {};
		for (Triangle tri : triangulation)
		{
			if (tri.indices.x == -1.0f) // OR Some complicated formula to calculate if
										// triangle is in circumcircle (TODO)
				badTriangles.push_back(tri);
		}
		
		std::vector<glm::vec2> polygon;
		for (Triangle tri : badTriangles)
		{
			for (int j = 0; j < 3; j++)
			{
				int start1 = j;
				int end1 = (j + 1) % 3;
				glm::vec2 currentEdge = glm::vec2(start1, end1);
				for (Triangle aTri : badTriangles)
				{
					for (int k = 0; k < 3; k++)
					{
						int start2 = k;
						int end2 = (k + 1) % 3;
						if ((tri.indices[start1] == aTri.indices[start2] && tri.indices[end1] == aTri.indices[end2]) ||
							(tri.indices[start1] == aTri.indices[end2] && aTri.indices[start2] == tri.indices[end1]))
							polygon.push_back(currentEdge);
					}
				}
			}
		}

		for (Triangle tri : badTriangles)
		{
			int end = triangulation.size() - 1;
			for (int j = 0; j < end; j++)
			{
				if (tri.indices == triangulation[j].indices)
				{
					triangulation.erase(triangulation.begin() + j);
					end--;
				}
			}
		}

		for (glm::vec2 edge : polygon)
		{
			Triangle newTri;
			newTri.indices = glm::vec3(edge[0], edge[1], i);
		}
	}

	int end = triangulation.size() - 1;
	for (int i = 0; i < end; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (triangulation[i].indices[j] == -1.0f)
			{
				triangulation.erase(triangulation.begin() + i);
				end--;
			}
		}
	}

	std::vector<unsigned int> triangulated;
	for (int i = 0; i < triangulation.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			triangulated.push_back(triangulation[i].indices[j]);
		}
	}
	return triangulated;
}