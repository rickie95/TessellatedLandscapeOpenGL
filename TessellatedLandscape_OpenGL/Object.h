#pragma once

#include <vector>
#include "Shader.h"
#include "CustomTypes.h"

class Object
{
private:
	float* data_f;
	std::vector<float3>* data;
	std::vector<uint3>* indices;
	
protected:
	unsigned int VAO = -1,
		VBO = -1,
		EBO = -1;
	GLenum drawMode;
	Shader* shader;
	void loadData(float* data, int size);
	void loadData(std::vector<float3>* data);
	Object();

public:
	Object(std::vector<float3>* vertices);
	Object(float* vertices, int size);
	~Object();
	void setIndices(std::vector<uint3>* ind);
	void setShader(Shader* shad);
	void sendUniform(const std::string & name, void* uniform)const;
	void setDrawMode(GLenum drawMode);
	void drawObject();
	Shader* useShader();
};

