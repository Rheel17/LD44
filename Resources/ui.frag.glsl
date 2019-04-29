#version 330 core

in vec2 io_Coordinates;

out vec4 frag_Color;

uniform vec4 color;
uniform bool hasTexture;
uniform sampler2D t;

void main(void) {
	if (hasTexture) {
		frag_Color = texture(t, io_Coordinates);
	} else {
		frag_Color = color;
	}
}
