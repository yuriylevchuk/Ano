#version 330 core

out vec4 color;

in vec2 texCoord;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;
uniform float mixingRatio;

void main() {
	color = mix(texture(ourTexture1, texCoord), texture(ourTexture2, texCoord), mixingRatio);
}