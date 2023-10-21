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
	class LUNA_API Texture
	{
	public:
		unsigned int id;
		unsigned int index;
		std::string type;
		const char* name;

		int width, height, nrComponents;
		GLenum format;
		unsigned char* data;

		Texture() {}

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

		static Texture getDefault();
		virtual void bind();
		virtual void unbind();

		static MaxSizeVector<Texture*, 16>
			generateFromPaths(std::vector<const char*> paths,
				std::vector<const char*> names);

	protected:
		int loadImage(const char* path);
		int loadTexture(const char* path, int index);
		int loadCubeMap(const char* path, int index);
	};
	
	class LUNA_API CubeMapTexture : public Texture
	{
	public:
		CubeMapTexture(const char* name, const char* path)
		{
			loadCubeMap(path, 0);
			this->name = name;
		}

		CubeMapTexture(const char* name, const char* path, int index)
		{
			loadCubeMap(path, index);
			this->name = name;
		}

		void bind() override;
		virtual void unbind();
	};

	struct PrimitiveVertex
	{
		glm::vec3 position = glm::vec3(1.0f);
		glm::vec3 color = glm::vec3(1.0f);
	};

	struct Vertex : public PrimitiveVertex
	{
		glm::vec3 normal = glm::vec3(1.0f);
		glm::vec2 texcoords = glm::vec3(0.0f);
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

	class LUNA_API Mesh : public Primitive
	{
	private:
		// vec3 + vec3 + vec3 + vec2
		static const int VERTEX_SIZE = 11;
		std::vector<float>			verticesFloatArray;
	public:
		std::vector<Vertex>			vertices;
		std::vector<unsigned int>	indices;
		MaxSizeVector<Texture*, 16>	textures;

		glm::vec3 center;

		void init(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
			MaxSizeVector<Texture*, 16>	textures);

		// Requires vertices to be triangulated
		Mesh();
		Mesh(const char* objPath);
		Mesh(std::vector<float> vertices);
		Mesh(std::vector<Vertex> vertices);
		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
		Mesh(std::vector<Vertex> vertices, MaxSizeVector<Texture*, 16>	textures);
		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
			MaxSizeVector<Texture*, 16>	textures);

		void setTextures(MaxSizeVector<Texture*, 16> textures);

		void draw(Frame frame, Shader shader);

		float* getFlatArray();
		glm::vec3 getCenter();
		static Mesh loadObjMesh(const char* filename);
		std::vector<unsigned int> triangulateMesh(std::vector<Vertex> vertices);
		static std::vector<Vertex> floatToVertex(std::vector<float> vertices);
		static std::vector<Vertex> createSquareArray();
		static Mesh createSquare();
	};

	class LUNA_API Model
	{
	protected:
		Shader shader;
		glm::vec3 position;
		glm::vec3 scale;
		glm::vec3 rotation;
		glm::mat4 model;
	public:
		Model(Shader shader, glm::vec3 position, glm::vec3 scale, glm::vec3 rotation);
		~Model();

		glm::mat4 getModelMatrix();

		void setRotation(glm::vec3 rotation);
		void setRotation(float xRot, float yRot, float zRot);

		void setPosition(glm::vec3 position);
		void setPosition(float x, float y, float z);

		Shader* getShader();
		virtual void draw(Frame frame);
	};

	class LUNA_API Object : public System, public Model
	{
	protected:
		Mesh mesh; // Give to Model
	public:
		Object(Mesh mesh, Shader shader, glm::vec3 position, glm::vec3 scale, glm::vec3 rotation);
		~Object();

		void frameUpdate(Frame frame) override;
		void tickUpdate(Tick tick) override;

		void draw(Frame frame) override;
		static Object createSquare();
		static Object createCube();
		static Object createSphere();

		void setTexture(std::string filename);
		void setCubeMap(std::string filename);
	};

	class Sprite : public Model
	{
		Sprite(Texture texture);
		void draw(Frame frame) override;
	};
}