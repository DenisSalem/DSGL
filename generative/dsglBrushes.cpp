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
							"	vec2 distance;\n"
							"	float minDistance = 16581375.0;\n"
							"	float tmp;\n"
							"	for (int i=0; i < 64; i++) {\n"
							"		distance.x = pixel.x - voronoiSeeds[i].x;\n"
							"		distance.y = pixel.y - voronoiSeeds[i].y;\n"
							"		tmp = distance.x * distance.x + distance.y * distance.y;\n"
							"		if (tmp < minDistance) {\n"
							"			minDistance = tmp;\n"
							"		}\n"
							"	}\n"
							"	float sqrtDistance = sqrt(minDistance) / ( float(brushScale) / 4.0 );\n"
							"	float factor = imageLoad(voronoi, ivec2(gl_GlobalInvocationID.xy)).x;\n"
							"	imageStore(voronoi, ivec2(gl_GlobalInvocationID.xy)+ivec2(brushScale,0), clamp(vec4(vec3(sqrtDistance * factor),1.0), vec4(0.0), vec4(1.0)));\n"
							"}";
		
		const char * Brushes::doubleVoronoiShader = "#version 430\n"
							"layout (local_size_x = 1, local_size_y = 1) in;\n"
							"layout (rgba32f, binding = 0) uniform image2D voronoi;\n"
							"uniform uint brushScale;\n"
							"uniform vec4 voronoiSeeds[64];\n"
							"const uint area = brushScale*brushScale;\n"
							"void main() {\n"
							"	vec2 pixel = vec2(gl_GlobalInvocationID.xy);\n"
							"	float minDistance1 = 16581375.0;\n"
							"	float minDistance2 = 16581375.0;\n"
							"	vec2 distance;\n"
							"	float tmp;\n"
							"	for (int i=0; i < 64; i++) {\n"
							"		distance.x = pixel.x - voronoiSeeds[i].x;\n"
							"		distance.y = pixel.y - voronoiSeeds[i].y;\n"
							"		tmp = distance.x * distance.x + distance.y * distance.y;\n"
							"		if (tmp < minDistance2) {\n"
							"			if ( tmp < minDistance1) {\n"
							"				minDistance2 = minDistance1;\n"
							"				minDistance1 = tmp;\n"
							"			}\n"
							"			else {\n"
							"				minDistance2 = tmp;\n"
							"			}\n"
							"		}\n"
							"	}\n"
							"	float sqrtDistance = sqrt(minDistance2 - minDistance1) / (float(brushScale) / 4 );\n"
							"	float factor = imageLoad(voronoi, ivec2(gl_GlobalInvocationID.xy)).x;\n"
							"	imageStore(voronoi, ivec2(gl_GlobalInvocationID.xy)+ivec2(brushScale*2,0), clamp(vec4(vec3(sqrtDistance * factor),1.0),vec4(0.0),vec4(1.0)));\n"
							"}";
		
		const char * Brushes::jellyVoronoiTesselationShader = "#version 430\n"
							"layout (local_size_x = 1, local_size_y = 1) in;\n"
							"layout (rgba32f, binding = 0) uniform image2D voronoi;\n"
							"uniform uint brushScale;\n"
							"uniform vec4 voronoiSeeds[64];\n"
							"const uint area = brushScale*brushScale;\n"
							"void main() {\n"
							"	vec2 pixel = vec2(gl_GlobalInvocationID.xy);\n"
							"	float minDistance1 = 16581375.0;\n"
							"	float minDistance2 = 16581375.0;\n"
							"	vec2 distance;\n"
							"	float tmp;\n"
							"	for (int i=0; i < 64; i++) {\n"
							"		distance.x = pixel.x - voronoiSeeds[i].x;\n"
							"		distance.y = pixel.y - voronoiSeeds[i].y;\n"
							"		tmp = distance.x * distance.x + distance.y * distance.y;\n"
							"		if (tmp < minDistance2) {\n"
							"			if ( tmp < minDistance1) {\n"
							"				minDistance2 = minDistance1;\n"
							"				minDistance1 = tmp;\n"
							"			}\n"
							"			else {\n"
							"				minDistance2 = tmp;\n"
							"			}\n"
							"		}\n"
							"	}\n"
							"	float sqrtDistance = (sqrt(minDistance1) * sqrt(minDistance2)) / (float(brushScale << 3) );\n"
							"	float factor = imageLoad(voronoi, ivec2(gl_GlobalInvocationID.xy)).x;\n"
							"	imageStore(voronoi, ivec2(gl_GlobalInvocationID.xy)+ivec2(brushScale*3,0), clamp(vec4(vec3(sqrtDistance * factor),1.0),vec4(0.0),vec4(1.0)));\n"
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
			
			// Jelly Voronoï tesselation
			
			for(int i = DSGL_GENERATIVE_VORONOI_CELLS * 2; i < DSGL_GENERATIVE_VORONOI_CELLS*3; i++) {
				voronoiSeeds[(i-DSGL_GENERATIVE_VORONOI_CELLS*2)*4] = ((short int *)seed)[i*3] % scale;	// x
				voronoiSeeds[((i-DSGL_GENERATIVE_VORONOI_CELLS*2)*4)+1] = ((short int *)seed)[i*5] % scale;	// y
			}
			
			DSGL::ComputeProgram jellyVoronoiTesselation(Brushes::jellyVoronoiTesselationShader, DSGL_READ_FROM_STRING);
			glUseProgram(jellyVoronoiTesselation.ID);
			jellyVoronoiTesselation.Uniformui("brushScale", scale);
			jellyVoronoiTesselation.Uniform4fv("voronoiSeeds", DSGL_GENERATIVE_VORONOI_CELLS, (GLfloat *) voronoiSeeds);
			glUseProgram(0);
			
			this->brushes->Bind();
        		jellyVoronoiTesselation.Use(this->scale, this->scale,1);
			glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
			this->brushes->Unbind();
		}
		Brushes::~Brushes() {}
	}
}
