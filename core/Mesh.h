#pragma once
#ifndef MESH_H
#define MESH_H

#include <string>
#include <iostream>

#include <thirdparty/glm/glm.hpp>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 texcoords;
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

};

#endif // !MESH_H
