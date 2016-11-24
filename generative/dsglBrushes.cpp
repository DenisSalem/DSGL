#include "dsgl.hpp"
#include "dsglBrushes.hpp"

namespace DSGL {
	namespace Generative {
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
							"	float distance = minDistance / ( float(brushScale) / 4 );\n"
							"	vec4 factor = imageLoad(voronoi, ivec2(gl_GlobalInvocationID.xy));\n"
							"	imageStore(voronoi, ivec2(gl_GlobalInvocationID.xy)+ivec2(brushScale,0), clamp(vec4(vec3(distance * factor),1.0), vec4(0.0), vec4(1.0)));\n"
							"}";
		
		const char * Brushes::doubleVoronoiShader = "#version 430\n"
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
							"	float secondMinDistance = 16581375.0;\n"
							"	int secondClosest=0;\n"
							"	for (int i=0; i < 64; i++) {\n"
	  						"		if(i != closest) {\n"
							"			tmp = sqrt(\n"
							"				pow(abs(pixel.x - voronoiSeeds[i].x),2) +\n"
							"				pow(abs(pixel.y - voronoiSeeds[i].y),2)\n"
							"			);\n"
							"			if (tmp < secondMinDistance) {\n"
							"				secondClosest=i;\n"
							"				secondMinDistance = tmp;\n"
							"			}\n"
							"		}\n"
							"	}\n"
							"	float distance = (secondMinDistance - minDistance) / (float(brushScale) / 4 );\n"
							"	vec4 factor = imageLoad(voronoi, ivec2(gl_GlobalInvocationID.xy));\n"
							"	imageStore(voronoi, ivec2(gl_GlobalInvocationID.xy)+ivec2(brushScale*2,0), clamp(vec4(vec3(distance * factor),1.0),vec4(0.0),vec4(1.0)));\n"
							"}";
		
		const char * Brushes::flatVoronoiTesselationShader = "#version 430\n"
							"layout (local_size_x = 1, local_size_y = 1) in;\n"
							"layout (rgba32f, binding = 0) coherent volatile uniform image2D voronoi;\n"
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
							"	float secondMinDistance = 16581375.0;\n"
							"	int secondClosest=0;\n"
							"	for (int i=0; i < 64; i++) {\n"
	  						"		if(i != closest) {\n"
							"			tmp = sqrt(\n"
							"				pow(abs(pixel.x - voronoiSeeds[i].x),2) +\n"
							"				pow(abs(pixel.y - voronoiSeeds[i].y),2)\n"
							"			);\n"
							"			if (tmp < secondMinDistance) {\n"
							"				secondClosest = i;\n"
							"				secondMinDistance = tmp;\n"
							"			}\n"
							"		}\n"
							"	}\n"
							"	float distance = sqrt(\n"
							"		pow(abs(voronoiSeeds[closest].x - voronoiSeeds[secondClosest].x),2) +\n"
							"		pow(abs(voronoiSeeds[closest].x - voronoiSeeds[secondClosest].y),2)\n"
							"	) / float(brushScale)  ;\n"
							"	vec4 factor = imageLoad(voronoi, ivec2(gl_GlobalInvocationID.xy));\n"
							"	imageStore(voronoi, ivec2(gl_GlobalInvocationID.xy)+ivec2(brushScale*3,0), vec4(vec3(distance * factor),1.0));\n"
							"}";

		Brushes::Brushes(unsigned int scale, char * seed) {
		  	this->scale = scale;
			
			this->brushes = std::make_shared<Textures>(GL_TEXTURE_2D, scale * DSGL_GENERATIVE_BRUSHES_MAX_NUMBER, scale, (GLvoid*)NULL);
			// Bell
			DSGL::ComputeProgram bell(Brushes::bellShader, DSGL_READ_FROM_STRING);
			glUseProgram(bell.ID);
			bell.Uniformui("brushScale", scale);
			glUseProgram(0);

			this->brushes->Bind();
        		bell.Use(this->scale, this->scale,1);
			glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
			this->brushes->Unbind();
			
			// Voronoï
			
			float voronoiSeeds[4*DSGL_GENERATIVE_VORONOI_CELLS] = {0};

			for(int i = 0; i < DSGL_GENERATIVE_VORONOI_CELLS; i++) {
				voronoiSeeds[(i*4)] = ((short int *)seed)[i] % scale;
				voronoiSeeds[(i*4)+1] = ((short int *)seed)[i*3] % scale;
			}

			DSGL::ComputeProgram voronoi(Brushes::voronoiShader, DSGL_READ_FROM_STRING);
			glUseProgram(voronoi.ID);
			voronoi.Uniformui("brushScale", scale);
			voronoi.Uniform4fv("voronoiSeeds", DSGL_GENERATIVE_VORONOI_CELLS, (GLfloat *) voronoiSeeds);
			glUseProgram(0);
			
			this->brushes->Bind();
        		voronoi.Use(this->scale, this->scale,1);
			glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
			this->brushes->Unbind();
			
			// Double Voronoï
	
			for(int i = DSGL_GENERATIVE_VORONOI_CELLS; i < DSGL_GENERATIVE_VORONOI_CELLS*2; i++) {
				voronoiSeeds[(i-DSGL_GENERATIVE_VORONOI_CELLS)*4] = ((short int *)seed)[i*5] % scale;		
				voronoiSeeds[((i-DSGL_GENERATIVE_VORONOI_CELLS))*4+1] = ((short int *)seed)[i*7] % scale;
			}

			DSGL::ComputeProgram doubleVoronoi(Brushes::doubleVoronoiShader, DSGL_READ_FROM_STRING);
			glUseProgram(doubleVoronoi.ID);
			doubleVoronoi.Uniformui("brushScale", scale);
			doubleVoronoi.Uniform4fv("voronoiSeeds", DSGL_GENERATIVE_VORONOI_CELLS, (GLfloat *) voronoiSeeds);
			glUseProgram(0);
			
			this->brushes->Bind();
        		doubleVoronoi.Use(this->scale, this->scale,1);
			glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
			this->brushes->Unbind();
			
			// Flat Voronoï tesselation
			
			for(int i = DSGL_GENERATIVE_VORONOI_CELLS * 2; i < DSGL_GENERATIVE_VORONOI_CELLS*3; i++) {
				voronoiSeeds[(i-DSGL_GENERATIVE_VORONOI_CELLS*2)*4] = ((short int *)seed)[i*3] % scale;	// x
				voronoiSeeds[((i-DSGL_GENERATIVE_VORONOI_CELLS*2)*4)+1] = ((short int *)seed)[i*5] % scale;	// y
			}
			
			DSGL::ComputeProgram flatVoronoiTesselation(Brushes::flatVoronoiTesselationShader, DSGL_READ_FROM_STRING);
			glUseProgram(flatVoronoiTesselation.ID);
			flatVoronoiTesselation.Uniformui("brushScale", scale);
			flatVoronoiTesselation.Uniform4fv("voronoiSeeds", DSGL_GENERATIVE_VORONOI_CELLS, (GLfloat *) voronoiSeeds);
			glUseProgram(0);
			
			this->brushes->Bind();
        		flatVoronoiTesselation.Use(this->scale, this->scale,1);
			glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
			this->brushes->Unbind();
		}
		Brushes::~Brushes() {
		//	DSGL_TRACE;
		}
	}
}
