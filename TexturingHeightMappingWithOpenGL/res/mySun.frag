#version 120

uniform sampler2D sunText;
varying vec2 sunTextCoord;
void main(void){
	
	gl_FragColor = texture2D(sunText,sunTextCoord)*vec4(0.1,0.5,1.0,1.0);

}
