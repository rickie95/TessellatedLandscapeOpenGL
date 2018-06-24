#version 430 core

in vec3 normal;

out vec4 FragColor;

uniform vec3 lightPos;

void main()
{
	vec3 objColor = vec3(1.0, 0.0, 1.0); // bianco
   //FragColor = vec4(ourColor, 1.0);


   float ambientStrenght = 0.3;
	vec3 ambient = ambientStrenght * vec3(1.0,1.0,1.0);

	//vec3 objColor = vec3(1.0, 0.0, 0.0);
	vec3 lightColor = vec3(1.0, 1.0, 1.0);
	
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - gl_FragCoord.xyz);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 result = (ambient + diffuse) * objColor;

   FragColor = vec4(result, 1.0);

}