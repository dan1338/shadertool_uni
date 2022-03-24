#version 140

in vec2 UV;
out vec4 COLOR;

uniform sampler2D frame;

void main()
{
	float dx = 1e-2;
	float dy = 1e-2;

	COLOR = 0.25 * (
		texture(frame, UV + vec2(-dx, 0.0)) + 
		texture(frame, UV + vec2(dx, 0.0)) + 
		texture(frame, UV + vec2(0.0, dy)) + 
		texture(frame, UV + vec2(0.0, -dy))
	);
}

