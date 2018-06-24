#version 410 core                                                                               
                                                                                                
layout(triangles, equal_spacing, ccw) in;               
layout(triangles) out;
                                                                                                
uniform mat4 view;
uniform mat4 projection;
//uniform sampler2D gDisplacementMap;                                                             
uniform float gDispFactor;      

uniform vec3 gEyeWorldPos;                                                                      
                                                                                                
in vec3 WorldPos_ES_in[];                                                                       
//in vec2 TexCoord_ES_in[];                                                                       
//in vec3 Normal_ES_in[];                                                                         
                                                                                                
out vec3 WorldPos_out;                                                                        
//out vec2 texCoord;                                                                        
out vec3 Normal;
out float distFromPos;
                                                                                            
vec2 interpolate2D(vec2 v0, vec2 v1, vec2 v2)                                                   
{                                                                                               
    return vec2(gl_TessCoord.x) * v0 + vec2(gl_TessCoord.y) * v1 + vec2(gl_TessCoord.z) * v2;   
}                                                                                               
                                                                                                
vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2)                                                   
{                                                                                               
    return vec3(gl_TessCoord.x) * v0 + vec3(gl_TessCoord.y) * v1 + vec3(gl_TessCoord.z) * v2;   
}

vec3 calculateNormal(vec3 v1, vec3 v2, vec3 v3)
{
   vec3 a = v1 - v2;
   vec3 b = v3 - v1;
   return normalize(cross(a, b));
}  
                                                                                                
void main()                                                                                     
{                                                                                               
    // Interpolate the attributes of the output vertex using the barycentric coordinates        
    //texCoord = interpolate2D(TexCoord_ES_in[0], TexCoord_ES_in[1], TexCoord_ES_in[2]);    
    //Normal = interpolate3D(Normal_ES_in[0], Normal_ES_in[1], Normal_ES_in[2]);
    //Normal = normalize(Normal);   
	
	Normal = calculateNormal(WorldPos_ES_in[0], WorldPos_ES_in[1], WorldPos_ES_in[2]);
    vec3 WorldPos = interpolate3D(WorldPos_ES_in[0], WorldPos_ES_in[1], WorldPos_ES_in[2]);    
                                                                                                
    // Displace the vertex along the normal                                                     
    //float Displacement = texture(gDisplacementMap, texCoord.xy).x;                        
    WorldPos += Normal * gDispFactor;  

	distFromPos = distance(WorldPos, gEyeWorldPos);

    WorldPos_out = (projection * view * vec4(WorldPos, 1.0)).xyz;                                              
} 