#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform sampler2D screenTexture;

out vec2 TexCoords;

void main()
{
	float col = texture(screenTexture, aTexCoords).r;
    TexCoords = aTexCoords;
    gl_Position = projection * view * model*vec4(aPos.x*5, 0.0,aPos.y*5,1.0); 
    //gl_Position = vec4(aPos,0.0,1.0); 
}  