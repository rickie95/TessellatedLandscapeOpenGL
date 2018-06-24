#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {

public:
	unsigned int ID;

	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geometryPath);
	Shader(const GLchar * vertexPath, const GLchar * fragmentPath, const GLchar * geometryPath, const GLchar* evaluationPath, const GLchar* controlPath);
	void use();
	void setData(const std::string &name, bool value) const;
	void setData(const std::string &name, int value) const;
	void setData(const std::string &name, float value) const;
	void setData(const std::string &name, glm::mat4 mat) const;
	void setData(const std::string &name, glm::vec3 vec) const;

private:
	const char* LoadFromFile(const char* filename);
	unsigned int compileShader(const char* shaderCode, char type);
};

#endif
