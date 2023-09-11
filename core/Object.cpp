#include "Object.h"

#include <thirdparty/glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <thirdparty/stb/stb_image.h>

#include "Camera.h"
#include "Space.h"

#include <glm/gtx/string_cast.hpp>

int Texture::loadTexture(const char* path)
{
	stbi_set_flip_vertically_on_load(true);
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
		format = formats[nrComponents-1];
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

Texture Texture::getDefault()
{
	return Texture("resources/textures/ash.png");
}

void Texture::bind()
{
	glBindTexture(GL_TEXTURE_2D, id);
}

Mesh::Mesh()
{}

Mesh::Mesh(std::vector<Vertex> vertices)
{
	this->vertices = vertices;
	toFloatArray();
	float* floatArrayPointer = &verticesFloatArray[0];
	this->indices = triangulateMesh(vertices);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 
		sizeof(float) * verticesFloatArray.size(), &verticesFloatArray[0], GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
		sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);

	/*
	for (int i = 0; i < verticesFloatArray.size(); i++)
	{
		std::cout << verticesFloatArray[i] << " ";
		if ((i + 1) % 8 == 0)
			std::cout << std::endl;
	}
	std::cout << std::endl;

	std::cout 
		<< sizeof(float) << " "
		<< verticesFloatArray.size() << " "
		<< sizeof(float) * verticesFloatArray.size() << " " 
		<< indices.size() << " "
		<< sizeof(float) * indices.size() << std::endl;
	*/

	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Vertex::SIZE * sizeof(float), 
		(void*)0);
	glEnableVertexAttribArray(0);

	// Normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, Vertex::SIZE * sizeof(float), 
		(void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Texture Coordinates
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, Vertex::SIZE * sizeof(float), 
		(void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

void Mesh::draw(RenderProps renderProps, Shader shader)
{
	shader.setMat4("model", renderProps.model);
	shader.setMat4("view", renderProps.view);
	shader.setMat4("projection", renderProps.proj);
	shader.use();
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::toFloatArray()
{
	int totalSize = vertices.size() * Vertex::SIZE;
	verticesFloatArray = std::vector<float>();
	for (int i = 0; i < vertices.size(); i++)
	{
		for (int j = 0; j < Vertex::SIZE; j++)
		{
			verticesFloatArray.push_back(
				vertices[i].getFloatArray()[j]
			);
		}
	}
}

Mesh Mesh::createSquare()
{
	std::vector<Vertex> vertices = {
		Vertex(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f),
					glm::vec2(0.0f, 0.0f)),
		Vertex(glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f),
					glm::vec2(0.0f, 1.0f)),
		Vertex(glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f),
					glm::vec2(1.0f, 0.0f)),
		Vertex(glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f),
					glm::vec2(1.0f, 1.0f))
	};

	return Mesh(vertices);
}

// This is a mess!
std::vector<unsigned int> Mesh::triangulateMesh(std::vector<Vertex> vertices)
{
	struct Triangle
	{
		glm::vec3 indices;

		glm::vec3 getPosition(int index, std::vector<Vertex> vers)
		{
			float inf = 10.0f;
			if (index == -3)
			{
				return glm::vec3(inf, -inf, 0);
			}
			else if (index == -2)
			{
				return glm::vec3(0, inf, 0);
			}
			else if (index == -1)
			{
				return glm::vec3(-inf, -inf, 0);
			}
			return vers[index].position;
		}
	};

	std::vector<Triangle> triangulation = {};
	Triangle superTriangle;
	superTriangle.indices = glm::vec3(-1.0f, -2.0f, -3.0f);
	triangulation.push_back(superTriangle);
	for (int i = 0; i < vertices.size(); i++)
	{
		Vertex ver = vertices[i];

		std::vector<Triangle> badTriangles = {};
		for (Triangle tri : triangulation)
		{
			glm::vec3 A = tri.getPosition(tri.indices[0], vertices);
			glm::vec3 B = tri.getPosition(tri.indices[1], vertices);
			glm::vec3 C = tri.getPosition(tri.indices[2], vertices);
			glm::vec3 D = ver.position;
			float circumcircleDeterminantArray[] = {
				A.x, A.y, A.x * A.x + A.y * A.y, 1.0f,
				B.x, B.y, B.x * B.x + B.y * B.y, 1.0f,
				C.x, C.y, C.x * C.x + C.y * C.y, 1.0f,
				D.x, D.y, D.x * D.x + D.y * D.y, 1.0f
			};
			glm::mat4 circumcircleDeterminant = glm::make_mat4(circumcircleDeterminantArray);
			float determinant = glm::determinant(circumcircleDeterminant);
			if (determinant < 0)
				badTriangles.push_back(tri);
		}

		std::vector<glm::vec2> polygon;
		for (int t1 = 0; t1 < badTriangles.size(); t1++)
		{
			Triangle tri = badTriangles[t1];
			for (int j = 0; j < 3; j++)
			{
				bool isShared = false;
				int start1 = j;
				int end1 = (j + 1) % 3;
				glm::vec2 currentEdge = glm::vec2(tri.indices[start1], tri.indices[end1]);
				for (int t2 = 0; t2 < badTriangles.size(); t2++)
				{
					if (t1 == t2)
						continue;
					Triangle aTri = badTriangles[t2];
					for (int k = 0; k < 3; k++)
					{
						int start2 = k;
						int end2 = (k + 1) % 3;
						if ((tri.indices[start1] == aTri.indices[start2] && tri.indices[end1] == aTri.indices[end2]) ||
							(tri.indices[start1] == aTri.indices[end2] && aTri.indices[start2] == tri.indices[end1]))
						{
							isShared = true;
						}
					}
				}
				if (!isShared)
					polygon.push_back(currentEdge);
			}
		}
		for (Triangle tri : badTriangles)
		{
			int end = triangulation.size();
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
			triangulation.push_back(newTri);
		}
		
	}

	int end = triangulation.size();
	for (int i = 0; i < end; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (triangulation[i].indices[j] < 0.0f)
			{
				triangulation.erase(triangulation.begin() + i);
				i--;
				end--;
				break;
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

Object::Object(Mesh mesh, Shader shader, glm::vec3 position, glm::vec3 scale)
{
	this->mesh = mesh;
	this->shader = shader;
	this->position = position;
	this->scale = scale;

	model = glm::mat4(1.0f);
	model = glm::scale(model, scale);
	model = glm::translate(model, position);
}

Object::~Object()
{
}

Shader* Object::getShader()
{
	return &shader;
}

void Object::draw(RenderProps renderProps)
{
	renderProps.model = model;
	mesh.draw(renderProps, shader);
}

Object Object::createSquare()
{
	Mesh mesh = Mesh::createSquare();
	Shader shader = Shader::getDefaultShader();

	return Object(mesh, shader, glm::vec3(0.0f), glm::vec3(1.0f));
}

void Sprite::draw(RenderProps renderProps)
{

}