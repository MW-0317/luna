#pragma once
#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>
#include <iostream>

#include <thirdparty/glm/glm.hpp>

#include "Shader.h"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 texcoords;
	Vertex(glm::vec3 pos, glm::vec3 nor, glm::vec3 tex) : position(pos), normal(nor), texcoords(tex)
	{}
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
public:
	std::vector<Vertex>			vertices;
	std::vector<unsigned int>	indices;
	std::vector<Texture>		textures;

	// Requires vertices to be triangulated
	Mesh(std::vector<Vertex> vertices, std::vector<Texture> textures);
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
		std::vector<Texture> textures);
	void Draw(Shader& shader);

	std::vector<unsigned int> triangulateMesh(std::vector<Vertex> vertices);
};

#endif // !MESH_H
