#version 140

in vec2 UV;
out vec4 COLOR;

uniform sampler2D aframe;
uniform sampler2D bframe;

void main() {
	COLOR = (texture(aframe, UV) + texture(bframe, UV)) * vec4(0.5);
}

