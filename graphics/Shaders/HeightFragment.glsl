#version 410 core

uniform sampler2D diffuseTex;
uniform sampler2D snowTex;
uniform sampler2D snowBumpTex;
uniform float 	  offset;
uniform sampler2D bumpTex;
uniform sampler2D specTex;
uniform sampler2D glossTex;
uniform sampler2D vertTex;
uniform sampler2D vertBumpTex;
uniform sampler2D vertSpecTex;

uniform vec3 	cameraPos;
uniform vec4 	lightEmiColour;
uniform vec4 	lightSpecColour;
uniform vec3 	lightPosition;
uniform vec3 	lightDirection;
uniform float 	lightRadius;
uniform float 	lightIntensity;
uniform float 	lightAngle;
uniform int 	lightType;

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

out vec4 fragColour;

void main(void)
{	
	float horizontality = clamp(pow(IN.normal.y * 0.5 + 0.5, 2), 0.0, 1.0);
	vec4 horizDiffuse	= texture(diffuseTex, IN.texCoord);
	vec4 vertDiffuse	= texture(vertTex, IN.texCoord);
	vec4 diffuse 		= mix(vertDiffuse, horizDiffuse, horizontality);
	
	mat3 TBN			= mat3(IN.tangent, IN.binormal, IN.normal);
	vec3 horizNormal 	= normalize(TBN * (texture(bumpTex, IN.texCoord).rgb * 2.0 - 1.0));
	vec3 vertNormal		= normalize(TBN * (texture(vertBumpTex, IN.texCoord).rgb * 2.0 - 1.0));
	vec3 normal 		= mix(vertNormal, horizNormal, horizontality);
	
	float snowCoverage 	= 0;
	snowCoverage 		= clamp((IN.height / 500) - (offset + 1.0), 0.0, 1.0);
	
	if (snowCoverage > 0)
	{
		diffuse 	= mix(diffuse, texture(snowTex, IN.texCoord), snowCoverage);
		normal 		= mix(normal, texture(snowTex, IN.texCoord).xyz, snowCoverage);
	}
	
	fragColour = vec4(0, 0, 0, 0);

	float dist 		= length(lightPosition - IN.worldPos);
	vec3 incident	= normalize(lightPosition - IN.worldPos);
	float lambert	= max(0.0, dot(incident, normal));
	
	float atten		= 1.0 - clamp(dist / lightRadius, 0.0, 1.0);
	vec3 viewDir	= normalize(cameraPos - IN.worldPos);
	vec3 halfDir	= normalize(incident + viewDir);
	
	float vertGloss = 0.33;
	float horizGloss = texture(glossTex, IN.texCoord).r;
	float gloss = mix(vertGloss, horizGloss, horizontality);
	
	float rFactor	= max(0.0, dot(halfDir, normal));
	float sFactor 	= pow(rFactor, gloss * 255);
	
	bool toBeLit = true;
	float spotAdjust = 1;

	if (lightType == 2)		//spotlight
	{
		float incidentAngle	= acos(dot(incident, normalize(lightDirection)));
		
		toBeLit 	= (lightAngle > abs(incidentAngle));
		spotAdjust 	= (pow(lightAngle, 3) - pow(abs(incidentAngle), 3)) / pow(lightAngle, 3);
	}
	else if (lightType == 1)	//directional
	{
		lambert	= max(0.0, dot(normalize(lightDirection), normal));
		
		atten = 1;
		halfDir	= normalize(lightDirection + viewDir);
	}
	vec3 horizSpecColour 	= texture(specTex, IN.texCoord).rgb;
	vec3 vertSpecColour		= texture(vertSpecTex, IN.texCoord).rgb;
	vec3 specColour 		= mix(vertSpecColour, horizSpecColour, horizontality);
	
	vec3 colour 		= (diffuse.rgb * lightEmiColour.rgb);
	colour 				+= (lightSpecColour.rgb * sFactor) * specColour;
	fragColour			+= float(toBeLit) * spotAdjust * vec4(colour * atten * lambert, diffuse.a);
	fragColour.rgb 		+= (diffuse.rgb * lightEmiColour.rgb) * 0.4; //ambient lighting
	
	//fragColour = vec4(horizontality);
	//fragColour.xyz = texture(bumpTex, IN.texCoord).rgb;
	//fragColour.a = 1.0;
}