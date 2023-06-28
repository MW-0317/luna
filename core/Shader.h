#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <thirdparty/glad/glad.h>
#include <thirdparty/glm/glm.hpp>
#include <thirdparty/glm/gtc/type_ptr.hpp>

class Shader
{
private:
	unsigned int id;

public:
	Shader(const char* vertexPath, const char* fragmentPath);
	void use();
	void setBool(const std::string& name, bool value);
	void setInt(const std::string& name, int value);
	void setFloat(const std::string& name, float value);
	void setFloat2(const std::string& name, float x, float y);
	void setFloat3(const std::string& name, float x, float y, float z);
	void setVec3(const std::string& name, glm::vec3 vector);
	void setMat4(const std::string& name, glm::mat4 matrix);
};