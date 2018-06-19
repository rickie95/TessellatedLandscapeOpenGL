#version 450 core
layout (location = 0) in vec3 aPos;


uniform mat4 model;

// AL GEOMETRY SHADER
out VS_OUT{
	vec3 fragPos;
} vs_out;

void main()
{
    // note that we read the multiplication from right to left
	//vs_out.fragPos = vec3(model*vec4(aPos,1.0));
	vs_out.fragPos = aPos;
    gl_Position = model*vec4(aPos,1.0);
}