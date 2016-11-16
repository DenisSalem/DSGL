#include "dsgl.hpp"

#ifndef DSGL_BRUSHES_INCLUDED
#define DSGL_BRUSHES_INCLUDED

#define DSGL_GENERATIVE_VORONOI_CELLS		64
#define DSGL_GENERATIVE_BRUSHES_MAX_NUMBER 	4
#define DSGL_GENERATIVE_BRUSH_BELL		0
#define DSGL_GENERATIVE_BRUSH_VORONOI		1
#define DSGL_GENERATIVE_BRUSH_DOUBLE_VORONOI	2
#define DSGL_GENERATIVE_BRUSH_FLAT_VORONOI	3

namespace DSGL {
	namespace Generative {
		struct Brushes {
		  	Brushes(unsigned int scale, char * seed);
			~Brushes();
		  	
			static const char * bellShader;
		  	static const char * voronoiShader;
		  	static const char * doubleVoronoiShader;
		  	static const char * flatVoronoiTesselationShader;
			
			unsigned int scale;
			
			std::shared_ptr<Textures> brushes;
		};
	}
}

#endif
