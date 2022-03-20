#version 140

in vec2 UV;
out vec4 COLOR;

uniform sampler2D frame;

void main()
{
	COLOR = texture(frame, UV);
}

