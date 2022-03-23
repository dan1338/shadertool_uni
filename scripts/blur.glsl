#version 140

in vec2 UV;
out vec4 COLOR;

uniform sampler2D frame;

void main()
{
	float dx = 3e-3;
	float dy = 3e-3;

	COLOR = 0.125 * (
		texture(frame, UV + vec2(-dx, dy)) +
		texture(frame, UV + vec2(0.0, dy)) +
		texture(frame, UV + vec2(dx, dy)) +
		texture(frame, UV + vec2(-dx, 0.0)) +
		texture(frame, UV + vec2(dx, 0.0)) +
		texture(frame, UV + vec2(dx, -dy)) +
		texture(frame, UV + vec2(0.0, -dy)) +
		texture(frame, UV + vec2(dx, -dy))
	);

	//COLOR = 0.25 * (
		//texture(frame, UV + vec2(-dx, 0.0)) +
		//texture(frame, UV + vec2(dx, 0.0)) +
		//texture(frame, UV + vec2(0.0, dy)) +
		//texture(frame, UV + vec2(0.0, -dy))
	//);
}

