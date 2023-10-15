#include "Shader.h"

#include <stdexcept>

namespace luna
{
	const std::vector<int> ShaderParser::SHADERS = {
		GL_VERTEX_SHADER,
		GL_TESS_CONTROL_SHADER,
		GL_GEOMETRY_SHADER,
		GL_FRAGMENT_SHADER,
		GL_COMPUTE_SHADER
	};

	ShaderParser::ShaderParser()
	{
		this->languageStack = std::vector<char>();
	}

	void ShaderParser::parseChar(size_t position, const char p, int currentShader)
	{
		int startIndex = -1;
		if (currentShader == GL_VERTEX_SHADER)
			startIndex = 0;
		else if (currentShader == GL_TESS_CONTROL_SHADER)
			startIndex = 1;
		else if (currentShader == GL_GEOMETRY_SHADER)
			startIndex = 2;
		else if (currentShader == GL_FRAGMENT_SHADER)
			startIndex = 3;
		else if (currentShader == GL_COMPUTE_SHADER)
			startIndex = 4;
		startIndex *= 2;

		if (p == '{')
		{
			if (this->isEmpty() && startIndex != -1)
				shaderPositions[startIndex] = position;
			languageStack.push_back(p);
		}
		else if (p == '}')
		{
			if (this->size() == 1 && startIndex != -1)
				shaderPositions[startIndex + 1] = position;
			languageStack.pop_back();
		}
	}

	void ShaderParser::parse(std::string shader)
	{
		int i = 0;
		int currentShader = 0;
		while (true)
		{
			char curr = shader[i];
			if (curr == '\0')
				throw std::invalid_argument("Please input a correct file. Parser read an empty file.");
			char next = shader[i + 1];
			this->parseChar(i, curr, currentShader);
			if (next == '\0')
				break;

			std::string cell = shader.substr(i, 2);
			if (cell == "VS" && this->isEmpty())
			{
				currentShader = GL_VERTEX_SHADER;
			}
			else if (cell == "FS" && this->isEmpty())
			{
				currentShader = GL_FRAGMENT_SHADER;
			}
			else if (cell == "CS" && this->isEmpty())
			{
				currentShader = GL_COMPUTE_SHADER;
			}
			i++;
		}

		vertex = shader.substr(shaderPositions[0] + 1, shaderPositions[1] - shaderPositions[0] - 1);
		fragment = shader.substr(shaderPositions[6] + 1, shaderPositions[7] - shaderPositions[6] - 1);
		compute = shader.substr(shaderPositions[8] + 1, shaderPositions[9] - shaderPositions[8] - 1);
	}

	bool ShaderParser::isEmpty()
	{
		return languageStack.empty();
	}

	int ShaderParser::size()
	{
		return languageStack.size();
	}

	std::string ShaderParser::getVertex()
	{
		return this->vertex;
	}

	std::string ShaderParser::getFragment()
	{
		return this->fragment;
	}

	std::string ShaderParser::getCompute()
	{
		return this->compute;
	}

	void Shader::compile(const char* vShaderCode, const char* fShaderCode)
	{
		unsigned int vertex, fragment;
		int success;
		char infoLog[512];

		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cout << "CORE::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
			return;
		}

		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cout << "CORE::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
			return;
		}

		this->id = glCreateProgram();
		glAttachShader(id, vertex);
		glAttachShader(id, fragment);
		glLinkProgram(id);
		glGetProgramiv(id, GL_LINK_STATUS, &success);

		if (!success)
		{
			glGetProgramInfoLog(id, 512, NULL, infoLog);
			std::cout << "CORE::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}

		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	Shader::Shader()
	{}

	Shader::Shader(const char* shaderPath)
	{
		std::string shaderCode;

		std::string vertexCode;
		std::string fragmentCode;
		std::string computeCode;

		const char* vShaderCode;
		const char* fShaderCode;
		const char* cShaderCode;

		std::ifstream shaderFile;

		ShaderParser s = ShaderParser();
		shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			shaderFile.open(shaderPath);
			std::stringstream shaderStream;
			shaderStream << shaderFile.rdbuf();
			shaderFile.close();
			shaderCode = shaderStream.str();

			s.parse(shaderCode);
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "CORE::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
		}

		vertexCode = s.getVertex();
		vShaderCode = vertexCode.c_str();

		fragmentCode = s.getFragment();
		fShaderCode = fragmentCode.c_str();

		computeCode = s.getCompute();
		cShaderCode = computeCode.c_str();

		this->compile(vShaderCode, fShaderCode);
	}

	Shader::Shader(const char* vertexPath, const char* fragmentPath)
	{
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;

			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			vShaderFile.close();
			fShaderFile.close();

			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "CORE::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
		}
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		this->compile(vShaderCode, fShaderCode);
	}

	void Shader::use()
	{
		glUseProgram(id);
	}

	void Shader::disable()
	{
		glUseProgram(0);
	}

	void Shader::setBool(const std::string& name, bool value)
	{
		glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
	}

	void Shader::setInt(const std::string& name, int value)
	{
		glUniform1i(glGetUniformLocation(id, name.c_str()), value);
	}

	void Shader::setFloat(const std::string& name, float x)
	{
		glUniform1f(glGetUniformLocation(id, name.c_str()), x);
	}

	void Shader::setFloat2(const std::string& name, float x, float y)
	{
		glUniform2f(glGetUniformLocation(id, name.c_str()), x, y);
	}

	void Shader::setFloat3(const std::string& name, float x, float y, float z)
	{
		glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z);
	}

	void Shader::setVec3(const std::string& name, glm::vec3 vector)
	{
		glUniform3f(glGetUniformLocation(id, name.c_str()), vector.x, vector.y, vector.z);
	}

	void Shader::setMat4(const std::string& name, glm::mat4 matrix)
	{
		glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
	}

	Shader Shader::getDefaultShader()
	{
		return Shader("shaders/example.glsl");
		//return Shader("shaders/basic.vs", "shaders/basic.fs");
	}

	Shader Shader::getBillboardShader()
	{
		return Shader("shaders/billboard.vs", "shaders/billboard.fs");
	}
}