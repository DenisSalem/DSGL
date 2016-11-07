#include "dsgl.hpp"
#include <cstdio>

namespace DSGL {
	namespace Images {
		typedef struct RGBA8 {
			unsigned char Red;      
			unsigned char Green;    
			unsigned char Blue;
			unsigned char Alpha;
		} rgba8;

		typedef struct RGBA32f {
			float Red;      
			float Green;    
			float Blue;
			float Alpha;
		} rgba32f;
	}
}

