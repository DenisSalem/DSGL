#include "dsgl.hpp"
#include <cstdio>

namespace DSGL {
	namespace Images {
		typedef struct RGB8 {
			GLubyte Red;      
			GLubyte Green;    
			GLubyte Blue;
			GLubyte Alpha;
		} rgb8;

		typedef struct RGBA8 {
			GLubyte Red;      
			GLubyte Green;    
			GLubyte Blue;
			GLubyte Alpha;
		} rgba8;

		typedef struct RGBA32f {
			GLfloat Red;      
			GLfloat Green;    
			GLfloat Blue;
			GLfloat Alpha;
		} rgba32f;
	}
}

