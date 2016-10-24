#include "dsglMeshes.hpp"

namespace DSGL {
	namespace Meshes {
	  	Triangle::Triangle() : Triangle(.0,.0,.0) {}
		
		Triangle::Triangle(GLfloat x, GLfloat y, GLfloat z) {
			this->vertex[0] = x+0.0;
			this->vertex[1] = y+1.0;	
			this->vertex[2] = z+0.0;
			this->vertex[3] = x-1.0;
			this->vertex[4] = y-1.0;	
			this->vertex[5] = z+0.0;
			this->vertex[6] = x+1.0;
			this->vertex[7] = y-1.0;	
			this->vertex[8] = z+0.0;

			this->index[0] = 0;
			this->index[1] = 1;
			this->index[2] = 2;
		}

	  	Quad::Quad() : Quad(.0,.0,.0) {}
		
		Quad::Quad(GLfloat x, GLfloat y, GLfloat z) {
			this->vertex[0] = x-1.0;
			this->vertex[1] = y+1.0;	
			this->vertex[2] = z+0.0;

			this->vertex[3] = x-1.0;
			this->vertex[4] = y-1.0;	
			this->vertex[5] = z+0.0;

			this->vertex[6] = x+1.0;
			this->vertex[7] = y+1.0;	
			this->vertex[8] = z+0.0;

			this->vertex[9] = x+1.0;
			this->vertex[10] = y-1.0;	
			this->vertex[11] = z+0.0;
			
			this->index[0] = 0;
			this->index[1] = 1;
			this->index[2] = 2;
			this->index[3] = 3;
		}
	}
}
