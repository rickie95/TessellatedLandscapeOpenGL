#version 450 core
layout (location = 0) in vec3 aPos;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// AL GEOMETRY SHADER

void main()
{
    // note that we read the multiplication from right to left
	//_out.fragPos = vec3(model*vec4(aPos,1.0));
    gl_Position = projection * view * model * vec4(aPos,1.0);
}