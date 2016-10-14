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
