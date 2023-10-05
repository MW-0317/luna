#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <thirdparty/glad/glad.h>
#include <thirdparty/glm/glm.hpp>
#include <thirdparty/glm/gtc/type_ptr.hpp>

#include "Core.h"

namespace luna
{
	class ShaderParser
	{
	private:
		std::vector<char> languageStack;

		/*
		* Formatted as followed:
		*	vertexStart,		vertexEnd,
		*	tessellationStart,	tessellationEnd,
		*	geometryStart,		geometryEnd
		*	fragmentStart,		fragmentEnd,
		*	computeStart,		computeEnd
		*/
		size_t shaderPositions[10];

		std::string vertex;
		std::string fragment;

	public:
		ShaderParser();

		void parseChar(size_t position, const char p, int currentShader);
		void parse(std::string shader);

		bool isEmpty();
		int size();

		std::string getVertex();
		std::string getFragment();
	};

	class Shader
	{
	private:
		unsigned int id;

		void compile(const char* vShaderCode, const char* fShaderCode);

	public:
		LUNA_API Shader();
		LUNA_API Shader(const char* shdaerPath);
		LUNA_API Shader(const char* vertexPath, const char* fragmentPath);
		void use();
		void disable();
		void setBool(const std::string& name, bool value);
		void setInt(const std::string& name, int value);
		void setFloat(const std::string& name, float value);
		void setFloat2(const std::string& name, float x, float y);
		void setFloat3(const std::string& name, float x, float y, float z);
		void setVec3(const std::string& name, glm::vec3 vector);
		void setMat4(const std::string& name, glm::mat4 matrix);

		LUNA_API static Shader getDefaultShader();
		LUNA_API static Shader getBillboardShader();
	};
}