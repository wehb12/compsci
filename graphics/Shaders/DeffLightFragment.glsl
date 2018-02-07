#version 150 core

uniform sampler2D depthTex;
uniform sampler2D normTex;

uniform vec2 pixelSize;
uniform vec3 cameraPos;

uniform float lightRadius;
uniform vec3 lightPosition;
uniform vec4 lightEmiColour;
uniform vec4 lightSpecColour;
uniform vec3 lightDirection;
uniform float lightAngle;
uniform int lightType;

in mat4 inverseProjView;
out vec4 gl_FragColor[2];

void main (void)
{
	vec3 pos 	= vec3((gl_FragCoord.x * pixelSize.x), (gl_FragCoord.y * pixelSize.y ), 0.0);
	pos.z 		= texture(depthTex, pos.xy).r;

	vec3 normal = normalize(texture(normTex, pos.xy).xyz * 2.0 - 1.0);

	vec4 clip 	= inverseProjView * vec4(pos * 2.0 - 1.0 , 1.0);
	pos 		= clip.xyz / clip.w;

	float dist 	= length(lightPosition - pos);
	float atten = 1.0 - clamp(dist / lightRadius, 0.0, 1.0);

	if (atten == 0.0)
	{
		discard;
	}

	vec3 incident = normalize(lightPosition - pos);
	vec3 viewDir = normalize(cameraPos - pos);
	vec3 halfDir = normalize(incident + viewDir);

	float lambert = clamp(dot(incident, normal), 0.0, 1.0);
	float rFactor = clamp(dot(halfDir, normal), 0.0, 1.0);
	float sFactor = pow(rFactor, 33.0);
	
	bool toBeLit = true;
	float spotAdjust = 1;

	if (lightType == 2)		//spotlight
	{
		float incidentAngle	= acos(dot(-incident, normalize(lightDirection)));
		
		toBeLit 	= (lightAngle > abs(incidentAngle));
		spotAdjust 	= (pow(lightAngle, 3) - pow(abs(incidentAngle), 3)) / pow(lightAngle, 3);
	}
	else if (lightType == 1)	//directional
	{
		lambert	= max(0.0, dot(normalize(lightDirection), normal));
		
		atten = 1;
		halfDir	= normalize(lightDirection + viewDir);
	}
	
	//vec3 diffuseColour 	= (diffuse.rgb * lightEmiColour.rgb);
	//vec3 specularColour	= (lightSpecColour.rgb * sFactor) * specColour;
	//vec3 ambient		= diffuseColour * 0.2;
	
	//vec3 colour 		= diffuseColour + specularColour;
	
	//fragColour[0]		+= vec4(float(toBeLit) * spotAdjust * shadow * atten * lambert * vec3(colour) + ambient, diffuse.a);
	
	//fragColour[1] 		= vec4(normal, 1.0);

	gl_FragColor[0] = vec4(lightEmiColour.xyz * /*lambert * */atten * float(toBeLit) * spotAdjust, 1.0);
	gl_FragColor[1] = vec4(lightSpecColour.xyz * sFactor * atten * float(toBeLit) * spotAdjust * 0.33, 1.0);
	
	//gl_FragColor[0] = vec4(vec3(atten),1.0);//vec4(1,0,0,1);
	//gl_FragColor[1] = vec4(0,1,0,1);
}
