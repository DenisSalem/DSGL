#include "dsgl.hpp"

#define DSGL_CUBEMAP_BRUSHES_MAX_NUMBER 1
#define DSGL_CUBEMAP_BRUSH_BELL 0

namespace DSGL {
	namespace Cubemap {
		struct Brushes {
		  	static const char * bellShader;
		  	Brushes(unsigned int scale);
			
			unsigned int scale;
			
			std::shared_ptr<Textures> brushes;
		};

	  	struct StamperOnCubeCorner {
		  	StamperOnCubeCorner(GLuint localFace, GLuint north, GLuint south, GLuint west, GLuint east);
			
			void SetBrushes(std::shared_ptr<Textures> brushes);
			void Stamp(unsigned int * coordsAttribs, unsigned int size);
		};
	}
}
