#version 330 core
out vec4 FragColor;

in vec3 ourNormal;
in vec3 FragPos;

uniform vec3 lightPos;

void main()
{
	float ambientStrenght = 0.1;
	vec3 ambient = ambientStrenght * vec3(1.0,1.0,1.0);

	vec3 objColor = vec3(1.0, 0.0, 0.0);
	vec3 lightColor = vec3(1.0, 1.0, 1.0);
	
	vec3 norm = normalize(ourNormal);
	vec3 lightDir = normalize(lightPos - FragPos);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 result = (ambient + diffuse) * objColor;

   FragColor = vec4(result, 1.0);
}