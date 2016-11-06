#version 430

layout (local_size_x = 1, local_size_y = 1) in;

layout (rgba32f, binding = 0) uniform image2D bell;

uniform uint brushScale; // should always be a power of two

float Interpolate(float t) {
	return 6 * pow(t,5) - 15 * pow(t,4) + 10 * pow(t,3);
}

vec3 GetElevation(uvec2 coord) {
  	int halfScale = int(brushScale >> 1);
  	int x = int(coord.x - halfScale);
	int y = int(coord.y - halfScale);
	float radius = sqrt(x*x + y*y) / halfScale;
	return vec3(1.0 - Interpolate(radius));
}

void main() {
	imageStore(bell, ivec2(gl_GlobalInvocationID.xy), vec4(GetElevation(gl_GlobalInvocationID.xy),1.0));
}
