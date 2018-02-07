#version 410 core

layout (triangles, equal_spacing, ccw) in;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;

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
} OUT;

float Combine1D(float f0, float f1, float f2)
{
	float x = gl_TessCoord.x;
	float y = gl_TessCoord.y;
	float z = gl_TessCoord.z;
	
	return x * f0 + y * f1 + z * f2;
}

vec2 Combine2D(vec2 v0, vec2 v1, vec2 v2)
{
	float x = gl_TessCoord.x;
	float y = gl_TessCoord.y;
	float z = gl_TessCoord.z;
	
	return x * v0 + y * v1 + z * v2;
}

vec3 Combine3D(vec3 v0, vec3 v1, vec3 v2)
{
	float x = gl_TessCoord.x;
	float y = gl_TessCoord.y;
	float z = gl_TessCoord.z;
	
	return x * v0 + y * v1 + z * v2;
}

vec4 Combine4D(vec4 v0, vec4 v1, vec4 v2)
{
	float x = gl_TessCoord.x;
	float y = gl_TessCoord.y;
	float z = gl_TessCoord.z;
	
	return x * v0 + y * v1 + z * v2;
}

void main(void)
{
	OUT.colour 			= Combine4D(IN[0].colour, IN[1].colour, IN[2].colour);
	OUT.specColour 		= Combine4D(IN[0].specColour, IN[1].specColour, IN[2].specColour);
	OUT.texCoord 		= Combine2D(IN[0].texCoord, IN[1].texCoord, IN[2].texCoord);
	vec3 tempNormal		= Combine3D(IN[0].normal, IN[1].normal, IN[2].normal);
	OUT.normal 			= normalize(tempNormal);
	vec3 tempTangent 	= Combine3D(IN[0].tangent, IN[1].tangent, IN[2].tangent);
	OUT.tangent 		= normalize(tempTangent);
	vec3 tempBinormal 	= Combine3D(IN[0].binormal, IN[1].binormal, IN[2].binormal);
	OUT.binormal 		= normalize(tempBinormal);
	vec3 tempWorldPos 	= Combine3D(IN[0].worldPos, IN[1].worldPos, IN[2].worldPos);
	OUT.height 			= Combine1D(IN[0].height, IN[1].height, IN[2].height);

	float dispY			= sin(100 * OUT.texCoord.x) * 100;
	gl_Position 		= (projMatrix * viewMatrix * modelMatrix) * vec4(tempWorldPos, 1.0);
	
	gl_Position.y 		+= dispY;
}