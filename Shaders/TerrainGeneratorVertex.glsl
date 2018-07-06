#version 330 core
layout (location = 0) in vec2 aPos;
l//ayout (location = 1) in vec2 in_texCord;

//out vec2 FragTexCoords;

void main()
{
	//FragTexCoords = in_texCord;
   gl_Position = vec4(aPos, 1.0,1.0);
}