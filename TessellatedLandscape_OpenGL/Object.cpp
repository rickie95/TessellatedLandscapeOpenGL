#include "Object.h"



void Object::loadData(float * Verts, int size)
{
	data_f = Verts;
	drawMode = GL_TRIANGLES;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	std::cout << sizeof(float) *size << std::endl;
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size, data_f, GL_STATIC_DRAW);
	glBindVertexArray(VAO);
	// location, attribute size, type, normalize, stride, offset
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void Object::loadData(std::vector<float3>* Verts)
{
	this->data = Verts;
	drawMode = GL_TRIANGLES;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float3)*data->size(), data->data(), GL_STATIC_DRAW);
	glBindVertexArray(VAO);
	// location, attribute size, type, normalize, stride, offset
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

Object::Object()
{
}

Object::Object(std::vector<float3>* vertices)
{
	loadData(vertices);
}

Object::Object(float* vertices, int size)
{
	loadData(vertices, size);
}


Object::~Object()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	if(EBO != -1)
		glDeleteBuffers(1, &EBO);
}

void Object::setIndices(std::vector<uint3>* ind)
{
	this->indices = ind;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint3)*indices->size(), indices->data(), GL_STATIC_DRAW);
}

void Object::setShader(Shader* shad)
{
	this->shader = shad;
}

void Object::sendUniform(const std::string & name, void * uniform) const
{
}

void Object::setDrawMode(GLenum drawMode)
{
	this->drawMode = drawMode;
}

void Object::drawObject()
{
	glBindVertexArray(VAO);
	if (EBO != -1) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glDrawElements(drawMode, indices->size() * 3, GL_UNSIGNED_INT, 0);
	}
	else {
		glDrawArrays(drawMode, 0, data->size());
	}
	glBindVertexArray(0);
}

Shader * Object::useShader()
{
	shader->use();
	return shader;
}

