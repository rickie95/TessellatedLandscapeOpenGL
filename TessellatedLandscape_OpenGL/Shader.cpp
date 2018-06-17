#include "Shader.h"

Shader::Shader(const GLchar * vertexPath, const GLchar * fragmentPath)
{
	const char* VertexCode = LoadFromFile(vertexPath);
	const char* FragmentCode = LoadFromFile(fragmentPath);

	unsigned int vertex = compileShader(VertexCode, 'v');
	unsigned int fragment = compileShader(FragmentCode, 'f');
		
	int success;
	char infoLog[512];
	ID = glCreateProgram();
	std::cout << "SHADER " << ID << " (" << vertexPath << " & " << fragmentPath << ")" << std::endl;
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	// print linking errors if any
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

Shader::Shader(const GLchar * vertexPath, const GLchar * fragmentPath, const GLchar * geometryPath)
{
	const char* VertexCode = LoadFromFile(vertexPath);
	const char* FragmentCode = LoadFromFile(fragmentPath);
	const char* GeometryCode = LoadFromFile(geometryPath);

	unsigned int vertex = compileShader(VertexCode, 'v');
	unsigned int fragment = compileShader(FragmentCode, 'f');
	unsigned int geometry = compileShader(GeometryCode, 'g');

	int success;
	char infoLog[512];
	ID = glCreateProgram();
	std::cout << "SHADER " << ID << " (" << vertexPath << " & " << fragmentPath << ")" << std::endl;
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glAttachShader(ID, geometry);
	glLinkProgram(ID);
	// print linking errors if any
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	glDeleteShader(geometry);
}

void Shader::use()
{
	glUseProgram(ID);
}

// Puttana che override dei metodi

void Shader::setData(const std::string & name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setData(const std::string & name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setData(const std::string & name, float value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
}

void Shader::setData(const std::string & name, glm::mat4 mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setData(const std::string & name, glm::vec3 vec) const{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &vec[0]);
}


const char* Shader::LoadFromFile(const char* filename)
{
	std::ifstream in(filename);
	std::string status;
	if (!in.good()) {
		std::cout << " File non esistente (" << filename << ")" << std::endl;
		return NULL;
	}

	std::string contents((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
	char *buffer = new char[contents.capacity()];
	strcpy_s(buffer, contents.capacity(), contents.c_str());
	if (buffer != "") {
		status = " Shader caricato correttamente (";
	}
	else {
		status = " We've got a situation here (";
		return NULL;
	}
	std::cout << status << filename << ")" << std::endl;
	return buffer;
}

unsigned int Shader::compileShader(const char * shaderCode, char type)
{
	unsigned int shader;
	int success;
	char infoLog[512];
	std::string shaderType;

	if (type == 'v') {
		shader = glCreateShader(GL_VERTEX_SHADER);
		shaderType = "VERTEX";
	}
	else if (type == 'f') {
		shader = glCreateShader(GL_FRAGMENT_SHADER);
		shaderType = "FRAGMENT";
	}
	else if (type == 'g') {
		shader = glCreateShader(GL_GEOMETRY_SHADER);
		shaderType = "FRAGMENT";
	}
	else {
		shaderType = "UNKNOW";
	}
	
	glShaderSource(shader, 1, &shaderCode, NULL);
	glCompileShader(shader);
	// print compile errors if any
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::"<< shaderType<<"::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	return shader;
}
