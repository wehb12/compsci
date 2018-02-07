#version 330 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 shadowMatrix;

uniform samplerBuffer weightTex;
uniform samplerBuffer transformTex;

in  vec3 position;

layout(location = 6) in vec2 weighting;

void main(void)	{
	int weightElements 	= int(weighting.x);
	int weightIndex 	= int(weighting.y);
	
	vec4 vertPos 	= vec4(0,0,0,1);
	
	for(int i = 0; i < weightElements; ++i) {	
		vec3 firstHalf  = texelFetch(weightTex,((weightIndex+i)*2)).rgb;
		vec3 weightPos  = texelFetch(weightTex,((weightIndex+i)*2)+1).rgb;
		
		mat4 jointTransform;
		jointTransform[0] = texelFetch(transformTex,(int(firstHalf.y)*8)+0);	//firstHalf.y = jointIndex
		jointTransform[1] = texelFetch(transformTex,(int(firstHalf.y)*8)+1);	//firstHalf.y = jointIndex
		jointTransform[2] = texelFetch(transformTex,(int(firstHalf.y)*8)+2);	//firstHalf.y = jointIndex
		jointTransform[3] = texelFetch(transformTex,(int(firstHalf.y)*8)+3);	//firstHalf.y = jointIndex
		
		mat4 invBindPose;
		invBindPose[0] = texelFetch(transformTex,(int(firstHalf.y)*8)+4);	//firstHalf.y = jointIndex
		invBindPose[1] = texelFetch(transformTex,(int(firstHalf.y)*8)+5);	//firstHalf.y = jointIndex
		invBindPose[2] = texelFetch(transformTex,(int(firstHalf.y)*8)+6);	//firstHalf.y = jointIndex
		invBindPose[3] = texelFetch(transformTex,(int(firstHalf.y)*8)+7);	//firstHalf.y = jointIndex	
			
		vertPos += (jointTransform * vec4(weightPos,1.0)) * firstHalf.z; 		// firstHalf.z = weightValue;
	}
	mat3 normalMatrix = transpose(mat3(modelMatrix));
	
	gl_Position		= (projMatrix * viewMatrix * modelMatrix) * vec4(vertPos.xyz, 1.0);
}