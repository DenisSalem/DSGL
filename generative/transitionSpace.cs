#version 430

layout (local_size_x = 1, local_size_y = 1) in;

layout (rgba32f, binding = 0) uniform image2D transitionSpace;
layout (rgba32f, binding = 1) uniform image2D brushes;
layout (rgba32ui, binding = 2) uniform image1D coords; // x, y, brushesID

uniform uint coordsNumber;	// How many time we want to stamp?
uniform uint brushesCount;	// How many brushes are availables?

uniform uint transitionSpaceScale;

uniform uint brusheScale;
uniform mat4 changeOfBasis;


void main() {
  	vec4 localElevation;
	for (int i = 0; i < coordsNumber; i++) {
		localElevation = imageLoad(brushes, gl_GlobalInvocationID.x, gl_GlobalInvocationID.y + brusheScale * imageLoad(coords,i).z);
		if (localElevation.x  > 0.0) { // Pixel is not null let's do the math!

		}
	}; 
}
