#pragma once
#include <string>
#include <vector>
#include <iostream>

#include <thirdparty/glm/glm.hpp>

#include "Core.h"
#include "Shader.h"
#include "System.h"
#include "effects/Effect.h"
#include "math/Vector.h"

namespace luna
{
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

	struct PrimitiveVertex
	{
		glm::vec3 position;
		glm::vec3 color;
	};

	struct Vertex : public PrimitiveVertex
	{
		glm::vec3 normal;
		glm::vec2 texcoords;
	};

	enum PrimitiveType
	{
		POINT,
		LINE,
		LINE_STRIP
	};

	class Primitive
	{
	protected:
		unsigned int VAO, VBO, EBO;
		PrimitiveType type;
		Shader basicShader;
	public:
		LUNA_API virtual void draw(Frame frame, Shader shader) {}
		LUNA_API Shader getBasicShader()
		{
			//return Shader("shaders/example.glsl");
			return basicShader;
		}
	};

	class Line : public Primitive
	{
	public:
		std::vector<PrimitiveVertex> points;
		std::vector<unsigned int> indices;

		void init(std::vector<PrimitiveVertex> points,
			std::vector<unsigned int> indices);
		LUNA_API Line(std::vector<PrimitiveVertex> points);
		LUNA_API Line(std::vector<glm::vec3> positions);
		LUNA_API Line(std::vector<glm::vec3> positions,
			std::vector<unsigned int> indices);
		void draw(Frame frame, Shader shader) override;

		float* getFlatArray()
		{
			if (points.size() == 0)
				return nullptr;
			return &points[0].position.x;
		}
	};

	class Mesh : public Primitive
	{
	private:
		// vec3 + vec3 + vec3 + vec2
		static const int VERTEX_SIZE = 11;
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
		LUNA_API Mesh(std::vector<float> vertices);
		LUNA_API Mesh(std::vector<Vertex> vertices);
		LUNA_API Mesh(std::vector<Vertex> vertices, MaxSizeVector<Texture, 16>	textures);
		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
			MaxSizeVector<Texture, 16>	textures);
		LUNA_API void draw(Frame frame, Shader shader);

		float* getFlatArray();
		std::vector<unsigned int> triangulateMesh(std::vector<Vertex> vertices);
		LUNA_API static std::vector<Vertex> floatToVertex(std::vector<float> vertices);
		LUNA_API static std::vector<Vertex> createSquareArray();
		LUNA_API static Mesh createSquare();
	};

	class LUNA_API Model
	{
	protected:
		Shader shader;
		glm::vec3 position;
		glm::vec3 scale;
		glm::mat4 model;
	public:
		Model(Shader shader, glm::vec3 position, glm::vec3 scale);
		~Model();

		Shader* getShader();
		virtual void draw(Frame frame);
	};

	class LUNA_API Object : public System, public Model
	{
	protected:
		Mesh mesh; // Change to primitive
	public:
		Object(Mesh mesh, Shader shader, glm::vec3 position, glm::vec3 scale);
		~Object();

		void frameUpdate(Frame frame) override;
		void tickUpdate(Tick tick) override;

		void draw(Frame frame) override;
		static Object createSquare();
	};

	class Sprite : public Model
	{
		Sprite(Texture texture);
		void draw(Frame frame) override;
	};
}