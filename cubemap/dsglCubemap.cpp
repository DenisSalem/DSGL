#include "dsglCubemap.hpp"

namespace DSGL {
	namespace Cubemap {
	  	const char * Brushes::bellShader =	"#version 430\n"
							"layout (local_size_x = 1, local_size_y = 1) in;\n"
							"layout (rgba32f, binding = 0) uniform image2D bell;\n"
							"uniform uint brushScale;\n"
							"\n"
							"float Interpolate(float t) {\n"
							"	return 6 * pow(t,5) - 15 * pow(t,4) + 10 * pow(t,3);\n"
							"}\n"
							"\n"
							"vec3 GetElevation(uvec2 coord) {\n"
  							"	int halfScale = int(brushScale >> 1);\n"
  							"	int x = int(coord.x - halfScale);\n"
							"	int y = int(coord.y - halfScale);\n"
							"	float radius = sqrt(x*x + y*y) / halfScale;\n"
							"	return vec3(1.0 - Interpolate(radius));\n"
							"}\n"
							"\n"
							"void main() {"
							"	imageStore(bell, ivec2(gl_GlobalInvocationID.xy), vec4(GetElevation(gl_GlobalInvocationID.xy),1.0));\n"
							"}\n";

		const char * Brushes::voronoiShader =	"#version 430\n"
							"\n";

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
			
			abcd32f voronoiSeeds[32] = {0};
			for(int i = 0; i < 32; i++) {
				voronoiSeeds[i].a = seed[i*i] % scale;			// x
				voronoiSeeds[i].b = seed[i] % scale;			// y
				voronoiSeeds[i].c = (GLfloat) seed[i+i] / 255.0;	// z
				voronoiSeeds[i].d = seed[4096-1-i] & 1;			// add or sub
			}

			DSGL::ComputeProgram voronoi("voronoi.cs", DSGL_READ_FROM_FILE);
			
			glUseProgram(voronoi.ID);

			voronoi.Uniformui("brushScale", scale);
			//voronoi.Uniform4fv("voronoiSeeds", 32, (GLfloat *) voronoiSeeds);
			
			glUseProgram(0);

			// Improved Perlin
		}
	}
}
