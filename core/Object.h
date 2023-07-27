#pragma once
#include <string>
#include <vector>
#include <iostream>

#include <thirdparty/glm/glm.hpp>

#include "Core.h"
#include "Shader.h"

struct Vertex
{
	static const int SIZE = 3 * 2 + 2;
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texcoords;

	float* floatArray = nullptr;

	Vertex(glm::vec3 pos) : position(pos)
	{
		normal = glm::vec3(0.0, 0.0, 1.0);
		texcoords = glm::vec2(0.0);
		toFloatArray();
	}
	Vertex(glm::vec3 pos, glm::vec3 nor, glm::vec2 tex) : position(pos), normal(nor), texcoords(tex)
	{
		toFloatArray();
	}

	~Vertex()
	{
		//freeFloatArray();
	}

	void toFloatArray()
	{
		floatArray = (float*) malloc(sizeof(float) * SIZE);
		if (!floatArray) abort();

		for (int i = 0; i < 3; i++)
		{
			floatArray[i] = glm::value_ptr(position)[i];
		}
		for (int i = 0; i < 3; i++)
		{
			floatArray[3 + i] = glm::value_ptr(normal)[i];
		}
		for (int i = 0; i < 2; i++)
		{
			floatArray[6 + i] = glm::value_ptr(texcoords)[i];
		}
	}

	void freeFloatArray()
	{
		if (floatArray) free(floatArray);
	}

	float* getFloatArray()
	{
		return floatArray;
	}
};

class Texture
{
public:
	unsigned int id;
	std::string type;

	Texture(char* path)
	{
		loadTexture(path);
	}
private:
	int loadTexture(char* path);
};

class Mesh
{
private:
	unsigned int VAO, VBO, EBO;
	std::vector<float>			verticesFloatArray;
public:
	std::vector<Vertex>			vertices;
	std::vector<unsigned int>	indices;
	std::vector<Texture>		textures;

	// Requires vertices to be triangulated
	Mesh();
	Mesh(std::vector<Vertex> vertices);
	Mesh(std::vector<Vertex> vertices, std::vector<Texture> textures);
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
		std::vector<Texture> textures);
	void draw(Shader shader);

	void toFloatArray();
	std::vector<unsigned int> triangulateMesh(std::vector<Vertex> vertices);

	
};

class Object
{
private:
	Mesh mesh;
	Shader shader;
	glm::vec3 position;
	glm::vec3 scale;
	glm::mat4 model;
public:
	LUNA_API Object(Mesh mesh, Shader shader, glm::vec3 position, glm::vec3 scale);
	LUNA_API ~Object();
	void draw(glm::mat4 view, glm::mat4 proj);
	LUNA_API static Object createSquare();
};