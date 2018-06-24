#version 430 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

out vec3 normal;

uniform mat4 view;
uniform mat4 projection;

in vec3 WorldPos_out[];


vec3 calculateNormal(vec3 v1, vec3 v2, vec3 v3)
{
   vec3 a = v1 - v2;
   vec3 b = v3 - v1;
   return normalize(cross(a, b));
}  

void main() {    
	normal = -calculateNormal(WorldPos_out[0], WorldPos_out[1], WorldPos_out[2]);
	
	// Creo i 3 vertici
    gl_Position = projection * view * vec4(WorldPos_out[0],1.0); 
    EmitVertex();
	gl_Position = projection * view * vec4(WorldPos_out[1],1.0);
	EmitVertex();
	gl_Position = projection * view * vec4(WorldPos_out[2],1.0);
	EmitVertex();

	// Sparo la normale
	
    EndPrimitive();
} 