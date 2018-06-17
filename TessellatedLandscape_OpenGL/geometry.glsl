#version 430 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

out vec3 normal;

uniform mat4 view;
uniform mat4 projection;

in VS_OUT{
	vec3 fragPos;
} gs_in[];

vec3 GetNormal()
{
   vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
   vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
   return normalize(cross(a, b));
}  

void main() {    
	normal = -GetNormal();
	
	// Creo i 3 vertici
    gl_Position = projection * view * gl_in[0].gl_Position; 
    EmitVertex();
	gl_Position = projection * view * gl_in[1].gl_Position;
	EmitVertex();
	gl_Position = projection * view * gl_in[2].gl_Position;
	EmitVertex();

	// Sparo la normale
	

    EndPrimitive();
} 