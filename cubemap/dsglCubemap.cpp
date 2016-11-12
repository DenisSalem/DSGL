#include "dsglCubemap.hpp"

#define DSGL_VORONOI_CELLS 64

namespace DSGL {
	namespace Cubemap {
	  	const char * Brushes::bellShader =	"#version 430\n"
							"layout (local_size_x = 1, local_size_y = 1) in;\n"
							"layout (rgba32f, binding = 0) uniform image2D bell;\n"
							"uniform uint brushScale;\n"
							"float Interpolate(float t) {\n"
							"	return 6 * pow(t,5) - 15 * pow(t,4) + 10 * pow(t,3);\n"
							"}\n"
							"vec3 GetElevation(uvec2 coord) {\n"
  							"	int halfScale = int(brushScale >> 1);\n"
  							"	int x = int(coord.x - halfScale);\n"
							"	int y = int(coord.y - halfScale);\n"
							"	float radius = sqrt(x*x + y*y) / halfScale;\n"
							"	return vec3(1.0 - Interpolate(radius));\n"
							"}\n"
							"void main() {"
							"	imageStore(bell, ivec2(gl_GlobalInvocationID.xy), vec4(GetElevation(gl_GlobalInvocationID.xy),1.0));\n"
							"}";

		const char * Brushes::voronoiShader =	"#version 430\n"
							"layout (local_size_x = 1, local_size_y = 1) in;\n"
							"layout (rgba32f, binding = 0) uniform image2D voronoi;\n"
							"uniform uint brushScale;\n"
							"uniform vec4 voronoiSeeds[64];\n"
							"const uint area = brushScale*brushScale;\n"
							"void main() {\n"
							"	vec2 pixel = vec2(gl_GlobalInvocationID.xy);\n"
							"	float minDistance = 16581375.0;\n"
							"	int closest=0;\n"
							"	float tmp;\n"
							"	for (int i=0; i < 64; i++) {\n"
							"		tmp = sqrt(\n"
							"			pow(abs(pixel.x - voronoiSeeds[i].x),2) +\n"
							"			pow(abs(pixel.y - voronoiSeeds[i].y),2)\n"
							"		);\n"
							"		if (tmp < minDistance) {\n"
							"			closest=i;\n"
							"			minDistance = tmp;\n"
							"		}\n"
							"	}\n"
							"	float distance = minDistance / ( float(brushScale) / 4.25 ); // Normalization is an approximation\n"
							"	vec4 factor = imageLoad(voronoi, ivec2(gl_GlobalInvocationID.xy));\n"
							"	imageStore(voronoi, ivec2(gl_GlobalInvocationID.xy)+ivec2(0,brushScale), vec4(distance * factor));\n"
							"}";
		
		const char * Brushes::doubleVoronoiShader =
							"#version 430\n";

		Brushes::Brushes(unsigned int scale, char * seed) {
		  	this->scale = scale;
			
			this->brushes = std::make_shared<Textures>(GL_TEXTURE_2D, scale, scale * DSGL_CUBEMAP_BRUSHES_MAX_NUMBER, (GLvoid*)NULL);
			
			// Bell
			
			DSGL::ComputeProgram bell(Brushes::bellShader, DSGL_READ_FROM_STRING);
			glUseProgram(bell.ID);
			bell.Uniformui("brushScale", scale);
			glUseProgram(0);

			this->brushes->Bind();
        		bell.Use(this->scale,this->scale,1);
			glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
			this->brushes->Unbind();

			// Voronoï
			
			abcd32f voronoiSeeds[DSGL_VORONOI_CELLS] = {0};
			for(int i = 0; i < DSGL_VORONOI_CELLS; i++) {
				voronoiSeeds[i].a = seed[(i*i)] % scale;		// x
				voronoiSeeds[i].b = seed[(i*i*i) % 4096] % scale;	// y
			}
			voronoiSeeds[0].c = 0.0;

			DSGL::ComputeProgram voronoi(Brushes::voronoiShader, DSGL_READ_FROM_STRING);
			glUseProgram(voronoi.ID);
			voronoi.Uniformui("brushScale", scale);
			voronoi.Uniform4fv("voronoiSeeds", DSGL_VORONOI_CELLS, (GLfloat *) voronoiSeeds);
			glUseProgram(0);
			
			this->brushes->Bind();
        		voronoi.Use(this->scale,this->scale,1);
			glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
			this->brushes->Unbind();

			// Double Voronoï
		}
	}
}
