#version 330 core

out vec4 frag_Color;

uniform vec4 color;

void main(void) {
	frag_Color = color;
}
