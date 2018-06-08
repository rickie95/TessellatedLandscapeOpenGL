#include <fstream>
#include <iostream>
#include <string>

#include "ShaderUtils.h"


void LoadShaderFromFile(const char * &shader, const char* filename)
{
	std::ifstream in(filename);
	std::string status;
	if (!in.good()) {
		std::cout << " File non esistente (" << filename << ")" << std::endl;
		return;

	}

	std::string contents((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
	char *buffer = new char[contents.capacity()];
	strcpy_s(buffer, contents.capacity(), contents.c_str());
	shader = buffer;
	if (buffer != "") {
		status = " Shader caricato correttamente (";
	}
	else {
		status = " We've got a situation here (";
	}
	std::cout << status << filename << ")" << std::endl;
}
