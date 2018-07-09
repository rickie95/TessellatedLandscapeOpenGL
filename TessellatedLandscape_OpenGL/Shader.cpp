#include "Shader.h"

Shader::Shader(const GLchar* vertexPath) {
	const char* VertexCode = LoadFromFile(vertexPath);
	unsigned int vertex = compileShader(VertexCode, 'v');
	ID = glCreateProgram();
	std::cout << "SHADER " << ID << " (" << vertexPath << ")" << std::endl;
	glAttachShader(ID, vertex);
	linkShaders(ID);
	glDeleteShader(vertex);
}

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

	ID = glCreateProgram();
	std::cout << "SHADER " << ID << " (" << vertexPath << " & " << fragmentPath << ")" << std::endl;
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glAttachShader(ID, geometry);

	linkShaders(ID);

	// delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	glDeleteShader(geometry);
}

Shader::Shader(const GLchar * vertexPath, const GLchar * fragmentPath, const GLchar * geometryPath, const GLchar* evaluationPath, const GLchar* controlPath)
{
	const char* VertexCode = LoadFromFile(vertexPath);
	const char* FragmentCode = LoadFromFile(fragmentPath);
	const char* GeometryCode = LoadFromFile(geometryPath);
	const char* EvaluationCode = LoadFromFile(evaluationPath);
	const char* ControlCode = LoadFromFile(controlPath);

	unsigned int vertex = compileShader(VertexCode, 'v');
	unsigned int fragment = compileShader(FragmentCode, 'f');
	unsigned int geometry = compileShader(GeometryCode, 'g');
	unsigned int evaluation = compileShader(EvaluationCode, 'e');
	unsigned int control = compileShader(ControlCode, 'c');

	
	ID = glCreateProgram();
	std::cout << "SHADER " << ID << " (" << vertexPath << " & " << fragmentPath << ")" << std::endl;
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glAttachShader(ID, geometry);
	glAttachShader(ID, evaluation);
	glAttachShader(ID, control);

	// print linking errors if any
	linkShaders(ID);

	// delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	glDeleteShader(geometry);
	glDeleteShader(evaluation);
	glDeleteShader(control);
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

void Shader::setData(const std::string & name, glm::vec4 vec) const
{
	glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &vec[0]);
}

void Shader::setData(const std::string & name, glm::vec3 vec) const{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &vec[0]);
}

void Shader::setData(const std::string & name, glm::vec2 vec) const
{
	glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &vec[0]);
}

void Shader::setData(const std::string & name, GLuint val) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), val);
}

void Shader::setFragDataLocation(const char *name, unsigned int val) {
	glBindFragDataLocation(this->ID, val, name);
}


void Shader::linkShaders(int ID)
{
	int success;
	char infoLog[512];
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
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
		status = "Shader caricato correttamente (";
	}
	else {
		status = "We've got a situation here (";
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
		shaderType = "GEOMETRY";
	}
	else if (type == 'c') {
		shader = glCreateShader(GL_TESS_CONTROL_SHADER);
		shaderType = "CONTROL";
	}
	else if (type == 'e') {
		shader = glCreateShader(GL_TESS_EVALUATION_SHADER);
		shaderType = "EVALUATION";
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
