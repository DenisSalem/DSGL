#version 430

layout (local_size_x = 1, local_size_y = 1) in;

layout (rgba32f,binding = 0)	uniform image2D surface;
layout (rgba32f,binding = 1)	uniform image2D brushes;
layout (r32ui,binding = 2)	uniform uimage1D seed;

uniform uint scale;
uniform uint depth;
uniform uint brushesNumber;

const int GOLDEN_NUMBER = 161803;
const int PI 		= 314159;
const int EULER 	= 271828;

ivec2	gridCoordinates;
ivec2 	randomBrush;
ivec2	offset;
uint	random;
uint	gridScale;
float	height;

uint GetRandom(int salt) {
	return imageLoad(seed, salt % 1024).x;
}

void SetScale(ivec2 coordinates) {
	gridCoordinates = coordinates;
	random = GetRandom( (PI * gridCoordinates.x) ^ (GOLDEN_NUMBER * gridCoordinates.y) ^ EULER );		
	randomBrush = ivec2( (random % (brushesNumber)) * scale, 0);
	offset = ivec2(
		-( gridScale >> 1 ) + (int(random) ^ (gridCoordinates.x + PI)) % gridScale,
		-( gridScale >> 1 ) + (int(random) ^ (gridCoordinates.y + EULER)) % gridScale
	);
}

void main() {
	ivec2	pixel = ivec2(gl_GlobalInvocationID.xy);
	
	for (int i = 0; i < depth; i++) {
	  	gridScale = scale >> i;
		SetScale(pixel / ivec2( gridScale ));

		if ( lessThan(pixel + offset, ivec2(scale)) == bvec2(true) && bvec2(true) == greaterThanEqual(pixel+offset, ivec2(0))) {
			height = imageLoad(brushes, pixel + offset + randomBrush).x;
			imageStore(surface, pixel, vec4(vec3(height), 1.0));
		}

		break;
	}
	
}
