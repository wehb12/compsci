#version 410 core

uniform sampler2D 		diffuseTex;
uniform sampler2D 		bumpTex;
uniform sampler2D 		specTex;
uniform sampler2DShadow shadowTex;

uniform int useTexture;

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
	vec4 shadowProj;
} IN;

out vec4 fragColour[2];

void main(void)
{
	vec4 diffuse 	= IN.colour;
	vec3 normal 	= IN.normal;
	vec3 specColour = vec3(1.0);
	
	if (useTexture > 0)
	{
		diffuse 	= texture(diffuseTex, IN.texCoord);
		specColour 	= texture(specTex, IN.texCoord).rgb;
		mat3 TBN	= mat3(IN.tangent, IN.binormal, IN.normal);
		normal 		= normalize(TBN * (texture(bumpTex, IN.texCoord).rgb * 2.0 - 1.0));
	}
	fragColour[0] = vec4(0, 0, 0, 0);

	float dist 		= length(lightPosition - IN.worldPos);
	vec3 incident	= normalize(lightPosition - IN.worldPos);
	float lambert	= max(0.0, dot(incident, normal));
	
	float atten		= 1.0 - clamp(dist / lightRadius, 0.0, 1.0);
	vec3 viewDir	= normalize(cameraPos - IN.worldPos);
	vec3 halfDir	= normalize(incident + viewDir);
	
	float rFactor	= max(0.0, dot(halfDir, normal));
	float sFactor 	= pow(rFactor, 50.0);
	
	bool toBeLit = true;
	float spotAdjust = 1;
	
	float shadow = 1.0;

	if (IN.shadowProj.w > 0.0)
	{
		shadow = textureProj(shadowTex, IN.shadowProj);
	}

	if (lightType == 2)		//spotlight
	{
		float incidentAngle	= acos(dot(-incident, normalize(lightDirection)));
		
		toBeLit 	= (lightAngle > abs(incidentAngle));
		spotAdjust 	= (pow(lightAngle, 3) - pow(abs(incidentAngle), 3)) / pow(lightAngle, 3);
		if (!toBeLit)
		{
			shadow = 1;
		}
	}
	else if (lightType == 1)	//directional
	{
		lambert	= max(0.0, dot(normalize(lightDirection), normal));
		
		atten = 1;
		halfDir	= normalize(lightDirection + viewDir);
	}
	
	// makes far away things fade to black
	float viewDist 		= length(cameraPos - IN.worldPos) / 2500;
	float zAdjust 		= clamp((viewDist), 0.0, 1.0);
	vec3 fadeToBlack   	= vec3(1 - zAdjust);
	
	vec3 diffuseColour 	= (diffuse.rgb * lightEmiColour.rgb);
	vec3 specularColour	= (lightSpecColour.rgb * sFactor) * specColour;
	vec3 ambient		= diffuseColour * 0.2;
	
	vec3 colour 		= (diffuseColour + specularColour)* atten * spotAdjust * float(toBeLit);
	
	fragColour[0]		= vec4(ambient + (diffuseColour * shadow), 1.0) * vec4(fadeToBlack, 1.0);
	
	fragColour[1] 		= vec4(normal * shadow, 1.0);
}