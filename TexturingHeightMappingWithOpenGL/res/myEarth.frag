#version 120

uniform sampler2D image;
varying vec2 textColor;
varying vec4 lightColor;

void main(void){
	gl_FragColor = texture2D(image,textColor)*lightColor;
}
