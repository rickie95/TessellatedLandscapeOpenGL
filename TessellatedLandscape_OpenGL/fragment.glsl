#version 330 core
out vec4 FragColor;
void main()
{
   vec3 ourColor = vec3(1.0, 0.0, 1.0);
   FragColor = vec4(ourColor, 1.0);
}