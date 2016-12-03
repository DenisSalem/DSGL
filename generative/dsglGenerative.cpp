#include "dsglGenerative.hpp"

namespace DSGL {
	namespace Generative {

		unsigned int SquareSurface::seedOffset = 42;

		SquareSurface::SquareSurface(GLuint scale, std::shared_ptr<Textures> brushes, const char * seed) {
			this->seed = seed;
			this->surface = std::make_shared<Textures>(GL_TEXTURE_2D, scale, scale, (GLvoid*) NULL, GL_RGBA_INTEGER, GL_UNSIGNED_INT, GL_RGBA32UI);

			GLuint defaultColor[4] = {4294967295,4294967295,4294967295,4294967295};
			//glClearTexImage(this->surface->textureID, 0, GL_RGBA_INTEGER, GL_UNSIGNED_INT, defaultColor);
			
			glFinish();

			GLuint color[4] = {0};
		DSGL_TRACE;
			this->surface->Bind(0);
			glReadPixels( 	0,
  				0,
				1,
				1,
				GL_RGBA,
				GL_UNSIGNED_INT,
				color);
			this->surface->Unbind();
			glFinish();
		DSGL_TRACE;

			std::cout << "Red: "<< (unsigned int) color[0] << "\n";
			std::cout << "Green: "<< (unsigned int) color[1] << "\n";
			std::cout << "BLue: "<< (unsigned int) color[2] << "\n";
			std::cout << "Alpha: "<< (unsigned int) color[3] << "\n";

			// GL_RED_INTEGER and NOT GL_RED: http://stackoverflow.com/questions/10058641/opengl-geometry-shader-integer-texture-fetch-fails
		      	Textures gpuSideSeed(GL_TEXTURE_1D, 4096 / sizeof(GLuint), 1, (GLvoid *) this->seed, GL_RED_INTEGER, GL_UNSIGNED_INT, GL_R32UI);
			
			DSGL::ComputeProgram generativeSurface("GenerativeSurface.cs", DSGL_READ_FROM_FILE);
			
			glUseProgram(generativeSurface.ID);
			generativeSurface.Uniformui("scale", scale);
			generativeSurface.Uniformui("depth", 8);
			generativeSurface.Uniformui("brushesNumber", 3);
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
