# version 330 core

uniform mat4 modelMatrix ;
uniform mat4 viewMatrix ;
uniform mat4 projMatrix ;
uniform mat4 shadowMatrix;

uniform samplerBuffer weightTex;
uniform samplerBuffer transformTex;

in  vec3 position;

layout(location = 6) in vec2 weighting;

void main ( void )
{
	gl_Position = (projMatrix * viewMatrix * modelMatrix) * vec4 (position , 1.0);
}
