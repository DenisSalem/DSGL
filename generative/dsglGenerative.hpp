#include "dsgl.hpp"
#include "dsglBrushes.hpp"

#ifndef DSGL_GENERATIVE_INCLUDED
#define DSGL_GENERATIVE_INCLUDED

namespace DSGL {
	namespace Generative {
	  	struct StamperOnCubeCorner {
		  	StamperOnCubeCorner(GLuint localFace, GLuint north, GLuint south, GLuint west, GLuint east);
			
			void SetBrushes(std::shared_ptr<Textures> brushes);
			void Stamp(unsigned int * coordsAttribs, unsigned int size);
		};

		class SquareSurface {
		  	public:
				SquareSurface(GLuint scale, std::shared_ptr<Textures> brushes, const char * seed);
				~SquareSurface();

				unsigned int GetRandomFromSeed(unsigned int salt);
				static unsigned int seedOffset;

				const char * seed;

				std::shared_ptr<Textures> surface;
				GLint * coords;
			private:
				void SetUpCoords(int scale, int offsetX, int offsetY, int realScale);
				
		};
	}
}

#endif
