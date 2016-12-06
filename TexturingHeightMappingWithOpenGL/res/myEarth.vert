#version 120
//
//	Lighting is done via the formulas given by the instructors examples at COW
//
uniform float change;
uniform sampler2D image2;
uniform vec3 lightPosition;
uniform float cameraPosX;
uniform float cameraPosY;
uniform float cameraPosZ;

varying vec2 textColor;
varying vec4 lightColor;

uniform mat4 myM;
uniform mat4 tMyM;


vec3 ambientK = vec3(0.25,0.25,0.25);
vec3 ambientI = vec3(0.3,0.3,0.3);
vec3 diffuseK = vec3(1.0,1.0,1.0);
vec3 diffuseI = vec3(1.0,1.0,1.0);
vec3 specK    = vec3(1.0,1.0,1.0);
vec3 specI    = vec3(1.0,1.0,1.0);
float alpha   = 100;

void main(void){


	textColor = gl_MultiTexCoord0.xy;
	
	vec4 myVec4 = texture2D(image2,textColor);
	
	vec4 myNor =  vec4(normalize(gl_NormalMatrix*gl_Normal),0.0);
	
	vec4 myPoint = myM * gl_Vertex;

	vec3 lVec = normalize((vec4(vec3(0,0,0),1.0)-myPoint).xyz);
	
	vec3 nVec = normalize((tMyM * vec4(gl_Normal,0)).xyz);

	vec3 vVec = normalize(vec3(cameraPosX,0.0,cameraPosZ)-myPoint.xyz);

	vec3 hVec = normalize(vVec + lVec);	

	gl_Position =  gl_ModelViewProjectionMatrix * gl_Vertex;
	
	gl_Position += myVec4*myNor*vec4(change,change,0.0,1.0);

	lightColor = vec4((ambientK*ambientI)+(diffuseK*diffuseI*max(0,dot(nVec,lVec)))+(specK*specI*pow(max(0,dot(nVec,hVec)),alpha)),1.0);

}
