#include "dsgl.hpp"

#define DSGL_CUBEMAP_BRUSHES_MAX_NUMBER 24
#define DSGL_CUBEMAP_BRUSHES_COUNT 1

namespace DSGL {
	namespace Cubemap {
		struct Brushes() {
		  	Brushes();
			GLuint brushes[DSGL_CUBEMAP_BRUSHES_MAX_NUMBER];
		};

	  	struct StamperOnCubeCorner {
		  	StamperOnCubeCorner(GLuint localFace, GLuint north, GLuint south, GLuint west, GLuint east);
			SetBrushes(Brushes * brushes);
			Stamp(unsigned int * coordsAttribs, unsigned int size);
		};
	}
}
