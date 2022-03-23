#version 140

in vec2 UV;
out vec4 COLOR;

uniform sampler2D aframe;
uniform sampler2D bframe;

void main() {
	vec2 uv = UV;

	if (uv.x < 0.5)
	{
		COLOR = texture(aframe, uv * vec2(2.0, 1.0));
	}
	else 
	{
		uv.x -= 0.5;
		COLOR = texture(bframe, uv * vec2(2.0, 1.0));
	}
}

