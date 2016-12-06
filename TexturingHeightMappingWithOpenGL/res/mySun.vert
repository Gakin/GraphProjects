#version 120
varying vec2 sunTextCoord;
void main(void){
	sunTextCoord = gl_MultiTexCoord0.xy;
	gl_Position =  gl_ModelViewProjectionMatrix * gl_Vertex;
	
}
