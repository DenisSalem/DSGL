#include "dsglGenerative.hpp"

namespace DSGL {
	namespace Generative {

		unsigned int SquareSurface::seedOffset = 42;

		SquareSurface::SquareSurface(GLuint scale, std::shared_ptr<Textures> brushes, const char * seed) {
			this->surface = std::make_shared<Textures>(GL_TEXTURE_2D, scale, scale, (GLvoid*) NULL);

			GLbyte zeroes[4] = {0};
			glClearTexImage(this->surface->textureID, 0, GL_RGBA, GL_UNSIGNED_BYTE, zeroes);
			this->seed = seed;

			int coordsSetSize = 0;
			for (int i = scale; i >= 2; i/=2) {
				coordsSetSize += scale*scale / (i*i);
			}

			// GL_RGB_INTEGER and NOT GL_RGB: http://stackoverflow.com/questions/10058641/opengl-geometry-shader-integer-texture-fetch-fails
		      	Textures gpuSideSeed(GL_TEXTURE_1D, 4096 / sizeof(GLuint), 1, (GLvoid *) this->seed, GL_RED_INTEGER, GL_UNSIGNED_INT, GL_R32UI);
			
			DSGL::ComputeProgram generativeSurface("GenerativeSurface.cs", DSGL_READ_FROM_FILE);
			
			glUseProgram(generativeSurface.ID);
			generativeSurface.Uniformui("scale", scale);
			//generativeSurface.Uniformui("depth", 8);
			//generativeSurface.Uniformui("brushesNumber", DSGL_GENERATIVE_VORONOI_CELLS);
			glUseProgram(0);


			this->surface->Bind(0);
			brushes->Bind(1);
			gpuSideSeed.Bind(2);
        		generativeSurface.Use(scale, scale,1);
			glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
			brushes->Unbind();
		}

		SquareSurface::~SquareSurface() {
		}
	}
}
