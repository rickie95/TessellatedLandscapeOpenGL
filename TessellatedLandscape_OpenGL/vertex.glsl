#version 450 core
layout (location = 0) in vec3 aPos;


uniform mat4 model;

// AL GEOMETRY SHADER
out vec3 WorldPos;

void main()
{
    // note that we read the multiplication from right to left
	//vs_out.fragPos = vec3(model*vec4(aPos,1.0));
	//WorldPos = aPos;
	//model*vec4(aPos,1.0);
    WorldPos = (model*vec4(aPos,1.0)).xyz;
}