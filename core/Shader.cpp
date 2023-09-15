#include "Shader.h"

ShaderStack::ShaderStack()
{
	this->languageStack = std::vector<char>();
}

void ShaderStack::parse(char p)
{
	if (p == '{')
		languageStack.push_back(p);
	else if (p == '}')
		languageStack.pop_back();
}

bool ShaderStack::isEmpty()
{
	return languageStack.empty();
}

int ShaderStack::size()
{
	return languageStack.size();
}

Shader::Shader()
{}

Shader::Shader(const char* shaderPath)
{
	std::string shaderCode;
	std::string vertexCode;
	std::string fragmentCode;
	const char* vShaderCode;
	const char* fShaderCode;
	const char* charShaderCode;
	std::ifstream shaderFile;
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		shaderFile.open(shaderPath);
		std::stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();
		shaderFile.close();
		shaderCode = shaderStream.str();
		charShaderCode = shaderCode.c_str();

		ShaderStack s;
		int i = 0;
		size_t vsStart;
		size_t vsEnd;
		size_t fsStart;
		size_t fsEnd;
		std::string currentShader = "";
		while (true)
		{
			char curr = shaderCode[i];
			char next = shaderCode[i + 1];
			s.parse(curr);

			if (curr == '\0' || next == '\0')
				break;

			std::string cell = shaderCode.substr(i, 2);
			if (cell == "VS" && s.isEmpty())
			{
				currentShader = "VS";
			}
			else if (cell == "FS" && s.isEmpty())
			{
				currentShader = "FS";
			}

			if (currentShader)
			i++;
		}
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "CORE::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
	}
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

void Shader::use()
{
	glUseProgram(id);
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
	return Shader("shaders/basic.vs", "shaders/basic.fs");
}

Shader Shader::getBillboardShader()
{
	return Shader("shaders/billboard.vs", "shaders/billboard.fs");
}