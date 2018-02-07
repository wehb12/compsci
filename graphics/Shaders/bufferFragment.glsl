#version 150 core

uniform sampler2D diffuseTex;
uniform sampler2D snowTex;
uniform float 	  offset;
uniform sampler2D bumpTex;
uniform sampler2D specTex;
uniform sampler2D glossTex;

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

out vec4 fragColour[2];

void main(void)
{	
	vec4 diffuse 	= texture(diffuseTex, IN.texCoord);
	vec3 normal 	= IN.normal;
	
	float snowCoverage = 0;
	snowCoverage = clamp((IN.height / 255) - offset, 0.0, 1.0);
	
	diffuse = mix(texture(diffuseTex, IN.texCoord), texture(snowTex, IN.texCoord), snowCoverage);

	mat3 TBN	= mat3(IN.tangent, IN.binormal, IN.normal);
	normal 		= mix(normalize(TBN * (texture(bumpTex, IN.texCoord).rgb * 2.0 - 1.0)), IN.normal, snowCoverage);

	fragColour[0] = diffuse;
	fragColour[1] = vec4(normal.xyz * 0.5 + 0.5, 1.0);
}