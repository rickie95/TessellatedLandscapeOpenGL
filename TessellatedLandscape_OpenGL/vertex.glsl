#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// AL GEOMETRY SHADER
out VS_OUT{
	vec3 FragPos;
} vs_out;

void main()
{
    // note that we read the multiplication from right to left
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vs_out.FragPos = vec3(model*vec4(aPos,1.0));
}