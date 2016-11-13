#version 430

layout (local_size_x = 1, local_size_y = 1) in;
layout (rgba32f, binding = 0) coherent volatile uniform image2D voronoi;

uniform uint brushScale;
uniform vec4 voronoiSeeds[64];

const uint area = brushScale*brushScale;

void main() {
	vec2 pixel = vec2(gl_GlobalInvocationID.xy); 
  	
	float minDistance = 16581375.0;
	int closest=0;
	float tmp;

	for (int i=0; i < 64; i++) {
		tmp = sqrt(
			pow(abs(pixel.x - voronoiSeeds[i].x),2) +
			pow(abs(pixel.y - voronoiSeeds[i].y),2)
		);
		if (tmp < minDistance) {
			closest=i;
			minDistance = tmp;
		}
	}

	float secondMinDistance = 16581375.0;
	int secondClosest=0;
	for (int i=0; i < 64; i++) {
	  	if(i != closest) {
			tmp = sqrt(
				pow(abs(pixel.x - voronoiSeeds[i].x),2) +
				pow(abs(pixel.y - voronoiSeeds[i].y),2)
			);

			if (tmp < secondMinDistance) {
				secondClosest = i;
				secondMinDistance = tmp;
			}
		}
	}



	float distance = sqrt(
		pow(abs(voronoiSeeds[closest].x - voronoiSeeds[secondClosest].x),2) +
		pow(abs(voronoiSeeds[closest].x - voronoiSeeds[secondClosest].y),2)
	) / float(brushScale)  ;
	
	distance  = distance < 0.25 ? 0 : distance;
	vec4 factor = imageLoad(voronoi, ivec2(gl_GlobalInvocationID.xy));
	imageStore(voronoi, ivec2(gl_GlobalInvocationID.xy)+ivec2(0,brushScale*3), (vec4(distance * factor)));
}
