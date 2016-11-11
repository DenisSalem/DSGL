#version 430

layout (local_size_x = 1, local_size_y = 1) in;
layout (rgba32f, binding = 0) uniform image2D voronoi;

uniform uint brushScale;
uniform vec4 voronoiSeeds[32];

void main() {
}
