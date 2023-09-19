#pragma once
#include <string>
#include <vector>
#include <iostream>

#include <thirdparty/glm/glm.hpp>

#include "Core.h"
#include "Shader.h"
#include "effects/Effect.h"
#include "math/Vector.h"

namespace luna
{
	class Space;
	class Camera;
	class Object;

	struct RenderProps
	{
		Space* space;
		Camera* camera;
		Object* object;

		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 proj;

		int width;
		int height;

		float deltatime;
	};

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
		Vertex(glm::vec3 pos, glm::vec3 nor) : position(pos), normal(nor)
		{
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
			floatArray = (float*)malloc(sizeof(float) * SIZE);
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

		static std::vector<Vertex> getSquareVector()
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
			return vertices;
		}
	};

	class Texture
	{
	public:
		unsigned int id;
		unsigned int index;
		std::string type;
		const char* name;

		Texture(const char* name, const char* path)
		{
			loadTexture(path, 0);
			this->name = name;
		}

		Texture(const char* name, const char* path, int index)
		{
			loadTexture(path, index);
			this->name = name;
		}

		LUNA_API static Texture getDefault();
		LUNA_API void bind();

		LUNA_API static MaxSizeVector<Texture, 16>
			generateFromPaths(std::vector<const char*> paths,
				std::vector<const char*> names)
		{
			MaxSizeVector<Texture, 16> textures;
			for (int i = 0; i < paths.size(); i++)
			{
				textures.push_back(Texture(names[i], paths[i], i));
			}

			return textures;
		}

	private:
		int loadTexture(const char* path, int index);
	};

	class Cell
	{
	protected:
		unsigned int VAO, VBO, EBO;
		Shader basicShader;
	public:
		LUNA_API virtual void draw(RenderProps renderProps, Shader shader) {}
		LUNA_API virtual Shader getBasicShader()
		{
			return Shader("example.glsl");
		}
	};

	struct LineVertex
	{
		glm::vec3 position;
		glm::vec3 color;
	};

	class Line : public Cell
	{
	public:
		float* flatarray;
		std::vector<LineVertex> points;

		void init(std::vector<LineVertex> points);
		LUNA_API Line(std::vector<LineVertex> points);
		LUNA_API Line(std::vector<glm::vec3> positions);
		void draw(RenderProps renderProps, Shader shader) override;

		Shader getBasicShader() override
		{
			return basicShader;
		}

		float* getFlatArray()
		{
			if (points.size() == 0)
				return nullptr;
			return &points[0].position.x;
		}
	};

	class Mesh : public Cell
	{
	private:
		std::vector<float>			verticesFloatArray;
	public:
		std::vector<Vertex>			vertices;
		std::vector<unsigned int>	indices;
		MaxSizeVector<Texture, 16>	textures;

		void init(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
			MaxSizeVector<Texture, 16>	textures);

		// Requires vertices to be triangulated
		LUNA_API Mesh();
		LUNA_API Mesh(const char* objPath);
		LUNA_API Mesh(std::vector<Vertex> vertices);
		LUNA_API Mesh(std::vector<Vertex> vertices, MaxSizeVector<Texture, 16>	textures);
		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
			MaxSizeVector<Texture, 16>	textures);
		LUNA_API void draw(RenderProps renderProps, Shader shader);

		void toFloatArray();
		std::vector<unsigned int> triangulateMesh(std::vector<Vertex> vertices);

		LUNA_API static Mesh createSquare();
	};

	class Object
	{
	protected:
		Mesh mesh;
		Shader shader;
		glm::vec3 position;
		glm::vec3 scale;
		glm::mat4 model;

		std::vector<Effect*> effects;
	public:
		LUNA_API Object(Mesh mesh, Shader shader, glm::vec3 position, glm::vec3 scale);
		LUNA_API ~Object();
		Shader* getShader();
		LUNA_API virtual void draw(RenderProps renderProps);
		LUNA_API static Object createSquare();

		LUNA_API void addEffect(Effect* effect);
	};

	class Sprite : public Object
	{
		LUNA_API Sprite(Texture texture);
		LUNA_API void draw(RenderProps renderProps) override;
	};
}