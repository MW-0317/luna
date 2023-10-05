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

	class Engine;
	class Space;
	class Camera;
	class Object;

	// NEED TO MOVE TO INTERVAL/SYSTEM
	struct RenderProps
	{
		Engine* engine;
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
		LUNA_API virtual void draw(RenderProps renderProps, Shader shader) {}
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
		void draw(RenderProps renderProps, Shader shader) override;

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