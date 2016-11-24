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
ivec2	gridCells;
ivec2 	randomBrush;
ivec2	offset;

uint	random;
uint	cellHalfScale;
uint	gridModulo;
float	height;

// ---- Some optimisation stuff ---- //

uint gridModulos[8] = {511,255,127,63,31,15,7,3}; 	// Avoiding modulos
uint gridCellsArray[8] = {1,2,4,8,16,32,64,128}; 	// Precomputed grid size
float gridDivisors[8] = {				// Avoiding divisions with precomputed coefficient
	0.001953125, 	// x / 512
	0.00390625,	// x / 256
	0.0078125,	// x / 128
	0.015625,	// x / 64
	0.03125,	// x / 32
	0.0625,		// x / 16
	0.125,		// x / 8
	0.25		// x / 4
};	

uint GetRandom(int salt) {
	return imageLoad(seed, salt & 2047).x; // modulo 1024
}

void SetStates(ivec2 coordinates) {
	random = GetRandom( (PI * gridCoordinates.x) ^ (GOLDEN_NUMBER * gridCoordinates.y) ^ EULER );
	randomBrush = ivec2( (random & brushesNumber) * scale, 0);
	offset = ivec2(									// By the way, when offset is negative,
		(((random + PI) ^ coordinates.x) & gridModulo) - cellHalfScale,		// let's say on the horizontal axis,
		(((random + EULER) ^ coordinates.y) & gridModulo) - cellHalfScale	// brush will actually be moved to the right.
	);
}

void main() {
	ivec2	pixel = ivec2(gl_GlobalInvocationID.xy);
	height = 0;
	for(int j = 0; j <1; j++) {
	for (int i = 0; i < depth; i++) {
		cellHalfScale = scale >> i;
		gridModulo = gridModulos[i];
		gridCoordinates = ivec2(pixel * gridDivisors[i]); // divide by current cell size


		//---- Current Cell ----//
		SetStates(gridCoordinates);
		if ( lessThan(pixel + offset, ivec2(scale)) == bvec2(true) && bvec2(true) == greaterThanEqual(pixel+offset, ivec2(0))) { // Be sure that we're not going beyond brush space.	
			height += imageLoad(brushes, pixel + offset + randomBrush).x;
		}
		break;
	}
	}
	imageStore(surface, pixel, vec4(vec3(height), 1.0));
	
}
