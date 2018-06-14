#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 ourNormal;
out vec3 FragPos;

void main()
{
    // note that we read the multiplication from right to left
    gl_Position = projection * view  * vec4(aPos, 1.0);
   ourNormal = aNormal;
   FragPos = vec3(model*vec4(aPos,1.0));
}