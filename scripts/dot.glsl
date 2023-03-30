#version 140

in vec2 UV;
out vec4 COLOR;

uniform sampler2D frame;

void main()
{
	vec2 uv = (UV - vec2(0.5)) * 2.0;
	float d = length(uv);
	if (d < 0.1)
		COLOR = vec4(1.0);
	else
		COLOR = texture(frame, UV);
}

