#version 330 core

layout(location = 0) in vec2 vert_Position;

out vec2 io_Coordinates;

uniform vec2 screenDimensions;
uniform vec4 params;

void main(void) {
	gl_Position.xy = params.xy + vert_Position * params.zw;
	
	// scale to correct resolution
	gl_Position.x *= screenDimensions.y / screenDimensions.x;
	
	// move to position
	gl_Position.x -= 1;
	gl_Position.y = 1 - gl_Position.y;
	
	gl_Position.zw = vec2(0.0, 1.0);
	
	io_Coordinates = vert_Position;
	
}
