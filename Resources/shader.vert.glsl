#version 330 core

layout(location = 0) in vec2 vert_Position;
layout(location = 1) in vec2 vert_TextureCoordinates;

uniform vec2 screenDimensions;
uniform vec2 location;
uniform float rotation;
uniform vec2 rotationCenter;
uniform vec3 cameraParams;

void main(void) {
	// set the position	
	gl_Position = vec4(vert_Position, 0.0, 1.0);
	
	// apply the rotation
	if (rotation != 0) {
		float s = sin(rotation);
		float c = cos(rotation);
		gl_Position.xy = rotationCenter + (mat2(c, -s, s, c) * (gl_Position.xy - rotationCenter));
	}
	
	// scale correctly
	gl_Position.xy /= 12.0;
	
	// move according to the position in the level
	gl_Position.x += (location.x / 24.0) * 2;
	gl_Position.y -= (location.y / 24.0) * 2;

	// apply the camera
	gl_Position.x -= cameraParams.x / 12.0;
	gl_Position.y += cameraParams.y / 12.0;
	gl_Position.xy *= cameraParams.z;
	
	// scale to correct resolution
	gl_Position.x *= screenDimensions.y / screenDimensions.x;
}
