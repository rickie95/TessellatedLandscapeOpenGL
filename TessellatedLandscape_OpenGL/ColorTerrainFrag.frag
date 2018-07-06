#version 330 core
out vec4 FragColor;

in vec2 TexCoords;


uniform sampler2D screenTexture;

void main()
{
	
    float col = texture(screenTexture, TexCoords+1/1024).r;
    FragColor = vec4(col, col, col, 1.0);
    //FragColor = vec4(1.0, 0.0, 1.0, 1.0);
} 