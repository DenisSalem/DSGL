#include "dsgl.hpp"

#define DSGL_CUBEMAP_BRUSHES_MAX_NUMBER 2
#define DSGL_CUBEMAP_BRUSH_BELL		0
#define DSGL_CUBEMAP_BRUSH_VORONOI	1


namespace DSGL {
	namespace Cubemap {
		struct abcd32f {
			GLfloat a;
			GLfloat b;
			GLfloat c;
			GLfloat d;
		};

		struct Brushes {
		  	static const char * bellShader;
		  	static const char * voronoiShader;
		  	Brushes(unsigned int scale, char * seed);
			
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
