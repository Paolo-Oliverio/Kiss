#version 450

in vec2 texcoord;
in vec4 color;

out vec4 FragColor;

uniform sampler2D texsampler;

void main() {
	vec4 colore = texture(texsampler, texcoord);
	FragColor =  colore * color ;
}