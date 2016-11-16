#version 430

layout (local_size_x = 1, local_size_y = 1) in;
layout (rgba32f, binding = 0) uniform image2D surface;
layout (rgba32f, binding = 1) uniform image2D brushes;
layout (rgba32i, binding = 2) uniform iimage1D coordinatesSet;

uniform uint scale;
uniform uint depth;
uniform uint brushesNumber;

void main() {
	ivec2 pixel = ivec2(gl_GlobalInvocationID.xy);
	vec4 height = vec4(0);
        int offsetIndex = 0;
	int localSize = 0;
	for (uint i = scale; i > 2; i/=2) { 
		offsetIndex += int(scale*scale / (i*i));
		localSize = int(scale*scale / ((i*i)/4)) - offsetIndex;

		for(int j = 0; j < localSize; j++) {
		  	ivec4 currentCoordinates = imageLoad(coordinatesSet,offsetIndex+j);
			if ( greaterThanEqual(pixel.xy, currentCoordinates.xy) == bvec2(true) && lessThan(pixel.xy, vec2(currentCoordinates.xy+int(scale))) == bvec2(true)) {
				vec4 height = imageLoad(brushes, pixel-currentCoordinates.xy+ivec2(scale,0));
				imageStore(surface, pixel, height);
			}
			return;
		}
	}
	
}
