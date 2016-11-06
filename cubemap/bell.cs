#version 430

layout (local_size_x = 1, local_size_y = 1) in;

layout (rgba32f, binding = 0) uniform image2D bell;

uniform uint brushScale; // should always be a power of two

float Interporlate(float t) {
	return 6 * pow(t,5) - 15 * pow(t,4) + 10 * pow(t,3);
}

vec3 GetElevation(uvec2 coord) {
	float radius = sqrt(pow(coord.x - (brushScale >> 1),2) + pow(coord.y - (brushScale >> 1),2)) / (brushScake >> 1)
	return 1.0 - Interpolate(radius);
}

void main() {
	imageStore(bell, coords, vec3(GetElevation(gl_GlobalInvocatioID.xy),1.0));
}
