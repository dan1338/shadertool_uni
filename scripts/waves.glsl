#version 140

in vec2 UV;
out vec4 COLOR;

uniform sampler2D frame;
uniform float time;

void main()
{
	vec2 uv = UV;
	uv.x += cos(uv.y * 6.283 + time) * 1e-1;
	COLOR = texture(frame, uv);
}

