#version 430

layout (local_size_x = 1, local_size_y = 1) in;

layout (rgba32f,binding = 0)	uniform image2D surface;
layout (rgba32f,binding = 1)	uniform image2D brushes;
layout (rgba32ui,binding = 2)	uniform uimage1D seed;

uniform uint scale;
uniform uint depth;
uniform uint brushesNumber;

const float GOLDEN_NUMBER = 1.6180339887;

uint GetRandom(int salt) {
	return imageLoad(seed, floatBitsToInt(GOLDEN_NUMBER * float(salt)) % 1024).x;
}

void main() {
	ivec2 pixel = ivec2(gl_GlobalInvocationID.xy);
	vec4 height = vec4(1);
	
	for (int d = 0; d < depth; d++) {
		if (d == 2 ) {
			break;
		}
		height +=
			(imageLoad(brushes,
				pixel + 
				ivec2(GetRandom(pixel.x*pixel.x) % ( scale >> 1) ,0) +
				ivec2(GetRandom(pixel.x*pixel.y) % brushesNumber ,0)
			)
		);
		height = imageLoad(brushes, pixel);
		break;
	}
	height /= 2;
	imageStore(surface, pixel, vec4(height.xyz, 1.0));
}
