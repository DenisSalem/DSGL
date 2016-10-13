#if defined(DSGL_GL3W)
	#include <GL/gl3w.h>
#else
	/* INCLUDE ALTERNATIVE LIBRARY HERE */
#endif

struct Triangle {
	GLfloat		vertex[9];
	GLuint		index[3];
	}
}

namespace DSGL {
	namespace Meshes {
		Triangle triangle = {
			{		
				0.0,	1.0,	0.0,
				-1.0,	-1.0,	0.0,
				1.0,	-1.0,	0.0
			},
			{0,1,2}
		};
	}
}
