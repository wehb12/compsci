#version 410 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;

in vec3 position;
in vec4 colour;
in vec4 specColour;
in vec3 normal;
in vec3 tangent;
in vec2 texCoord;

out Vertex
{
	vec4 colour;
	vec4 specColour;
	vec2 texCoord;
	vec3 normal;
	vec3 tangent;
	vec3 binormal;
	vec3 worldPos;
	float height;
} OUT;

void main(void)
{
	OUT.colour 			= colour;
	OUT.specColour		= specColour;
	OUT.texCoord 		= (textureMatrix * vec4(texCoord, 0.0, 1.0)).xy;
	
	mat3 normalMatrix 	= transpose(inverse(mat3(modelMatrix)));
	
	OUT.normal			= normalize(normalMatrix * normalize(normal));
	OUT.tangent			= normalize(normalMatrix * normalize(tangent));
	OUT.binormal		= normalize(normalMatrix * normalize(cross(normal, tangent)));
	
	OUT.worldPos		= position;
	gl_Position			= vec4(position, 1.0);
	
	OUT.height 			= position.y;
}