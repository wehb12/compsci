#version 150 core

uniform sampler2D 	diffuseTex;
uniform sampler2D 	bumpTex;
uniform samplerCube cubeTex;

uniform vec4 	lightEmiColour;
uniform vec4 	lightSpecColour;
uniform vec3 	lightPosition;
uniform float 	lightRadius;
uniform int 	lightType;
uniform vec3 	lightDirection;
uniform float 	lightAngle;
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
	gl_FragColor = vec4(0, 0, 0, 0);

	vec4 diffuse 	= texture(diffuseTex, IN.texCoord) * IN.colour;
	float dist 		= length(lightPosition - IN.worldPos);
	vec3 incident	= normalize(lightPosition - IN.worldPos);
	
	mat3 TBN		= mat3(IN.tangent, IN.binormal, IN.normal);
	vec3 normal 	= normalize(TBN * texture(bumpTex, IN.texCoord).xyz);
	// normals generated using online app, and need some adjustment
	normal.x        = normal.x - 0.4;
	normal.z        = normal.z + 0.35;
	float lambert	= max(0.0, dot(incident, normal));
	
	float atten		= 1.0 - clamp(dist / lightRadius, 0.0, 1.0);
	vec3 viewDir	= normalize(cameraPos - IN.worldPos);
	vec3 halfDir	= normalize(incident + viewDir);
	
	float rFactor	= max(0.0, dot(halfDir, normal));
	float sFactor 	= pow(rFactor, 50);
	
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
	vec3 reflIncident 	= normalize(IN.worldPos - cameraPos);
	vec4 reflection 	= texture(cubeTex, reflect(reflIncident, normalize(normal)));
	
	vec3 colour 		= (diffuse.rgb * lightEmiColour.rgb);
	colour 				+= (lightSpecColour.rgb * sFactor);
	gl_FragColor		+= float(toBeLit) * spotAdjust * vec4(colour * atten * lambert, diffuse.a) * vec4((diffuse + reflection).xyz, 0.5);
	gl_FragColor.rgb 	+= (diffuse.rgb * lightEmiColour.rgb) * 0.1; //ambient lighting

	gl_FragColor.rgb 	+= vec3(((lightEmiColour * diffuse * atten)*(diffuse + reflection)).xyz);
}