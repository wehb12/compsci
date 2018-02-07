#version 330 core

uniform sampler2D 		diffuseTex;
uniform sampler2D 		bumpTex;
uniform sampler2DShadow shadowTex;

uniform vec4 lightEmiColour;
uniform vec3 lightPosition;
uniform vec3 cameraPos;
uniform float lightRadius;
uniform vec3 lightDirection;
uniform int lightType;
uniform float lightAngle;

in Vertex
{
	vec4 	colour;
	vec4 	specColour;
	vec2 	texCoord;
	vec3 	normal;
	vec3 	tangent;
	vec3 	binormal;
	vec3 	worldPos;
	vec4 	shadowProj;
} IN;

out vec4 gl_FragColor ;

void main ( void )
{
	mat3 TBN = mat3 ( IN.tangent , IN.binormal , IN.normal );
	vec3 normal = normalize ( TBN * ( texture2D ( bumpTex , IN.texCoord).rgb * 2.0 - 1.0));

	vec4 diffuse = texture ( diffuseTex , IN . texCoord );
	
	vec3 incident = normalize ( lightPosition - IN.worldPos );
	float lambert = max (0.0 , dot ( incident, normal ));

	float dist = length ( lightPosition - IN.worldPos );
	float atten = 1.0f;//1.0 - clamp ( dist / lightRadius[0] , 0.0 , 1.0);

	vec3 viewDir = normalize ( cameraPos - IN . worldPos );
	vec3 halfDir = normalize ( incident + viewDir );

	float rFactor = max (0.0 , dot ( halfDir , normal ));
	float sFactor = pow ( rFactor , 33.0 );

	bool toBeLit = (dist < lightRadius);
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
		
		toBeLit = true;
	}
	
	float shadow = 1.0;

	if (IN.shadowProj.w > 0.0)
	{
		shadow = textureProj(shadowTex, IN.shadowProj);
		//shadow = 0;
	}
	
	float viewDist 		= length(cameraPos - IN.worldPos) / 2500;
	float zAdjust 		= clamp((viewDist), 0.0, 1.0);
	vec3 fadeToBlack   	= vec3(1 - zAdjust);

	vec3 col = diffuse.rgb;
	vec3 diffuse_col = col * lambert;
	vec3 spec_col = col * sFactor * 0.33;
	vec3 ambient = diffuse.rgb * 0.1f;
	
	gl_FragColor.xyz = ambient + (diffuse_col + spec_col) * float(toBeLit) * spotAdjust * shadow;
	gl_FragColor.a = 1.0f;
	gl_FragColor = gl_FragColor * vec4(fadeToBlack, 1.0);
	
	// vec2 tc_s = (IN.shadowProj.xy / IN.shadowProj.w) * 2.0f - 1.0f;
	// if (abs(tc_s.x) <= 1.0f && abs(tc_s.y) <= 1.0f)
	// {
		// tc_s = tc_s * 0.5f + 0.5f;
		// gl_FragColor.xy = tc_s;
	// }
	
	//gl_FragColor = vec4(shadow, shadow, shadow, 1);
	//gl_FragColor = vec4(vec3(spotAdjust), 1);
}