#version 150 core

uniform samplerCube cubeTex;

uniform vec3 	cameraPos;

in Vertex
{
	vec4 colour;
	vec4 specColour;
	vec2 texCoord;
	vec3 normal;
	vec3 tangent;
	vec3 binormal;
	vec3 worldPos;
	float height;
} IN;

out vec4 gl_FragColor;

void main (void)
{
	vec3 reflIncident 	= normalize(IN.worldPos - cameraPos);
	vec4 reflection 	= texture(cubeTex, reflect(reflIncident, normalize(IN.normal)));
	gl_FragColor 		= reflection;
}