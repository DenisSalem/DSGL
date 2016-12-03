#version 430

layout (local_size_x = 1, local_size_y = 1) in;

layout (rgba32f,binding = 0)	uniform image2D surface;
layout (rgba32f,binding = 1)	uniform image2D brushes;
layout (r32ui,binding = 2)	uniform uimage1D seed;

uniform uint scale;
uniform uint depth;
uniform uint brushesNumber;

const int GOLDEN_NUMBER = 16180339;

uint GetRandom(int salt) {
	return imageLoad(seed, salt % 1024).x;
}

void main() {
	ivec2 pixel = ivec2(gl_GlobalInvocationID.xy);

	float height = float(GetRandom(pixel.x ^ GOLDEN_NUMBER ^ pixel.y)) / float(scale);
	
	imageStore(surface, pixel, vec4(vec3(height), 1.0));
}
