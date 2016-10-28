#include "dsgl.hpp"

namespace DSGL {
	namespace Cubemap {
	  	struct StamperOnCubeCorner{
		  	StamperOnCubeCorner(GLuint localFace);

			Stamp(Gluint horizontalNeighbourFace, Gluin verticalNeighbourFace, GLuint stamp, unsigned int x, unsigned y, unsigned int octave);
			
			GLuint stampShaderID;
  			GLuint stampProgramID;

			const char * stampShaderSource = ""; 
		}
	}
}
