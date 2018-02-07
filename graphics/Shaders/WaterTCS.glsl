#version 410 core

layout (vertices = 3) out;

uniform vec3 cameraPos;
uniform vec4 tessLevel;

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
} IN[];

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
} OUT[];

void main(void)
{
	OUT[gl_InvocationID].colour = IN[gl_InvocationID].colour;
	OUT[gl_InvocationID].specColour = IN[gl_InvocationID].specColour;
	OUT[gl_InvocationID].texCoord = IN[gl_InvocationID].texCoord;
	OUT[gl_InvocationID].normal = IN[gl_InvocationID].normal;
	OUT[gl_InvocationID].tangent = IN[gl_InvocationID].tangent;
	OUT[gl_InvocationID].binormal = IN[gl_InvocationID].binormal;
	OUT[gl_InvocationID].worldPos = IN[gl_InvocationID].worldPos;
	OUT[gl_InvocationID].height = IN[gl_InvocationID].height;
	
	gl_TessLevelOuter[0] = tessLevel.x;
	gl_TessLevelOuter[1] = tessLevel.y;
	gl_TessLevelOuter[2] = tessLevel.z;
	gl_TessLevelInner[0] = tessLevel.w;
}