#include "dsglGenerative.hpp"

namespace DSGL {
	namespace Generative {

		unsigned int SquareSurface::seedOffset = 42;

		SquareSurface::SquareSurface(GLuint scale, std::shared_ptr<Textures> brushes, const char * seed) {
			DSGL_TRACE; std::cout << "Surface\n";
			this->surface = std::make_shared<Textures>(GL_TEXTURE_2D, scale, scale, (GLvoid*) NULL);
			DSGL_TRACE; std::cout << "Surface\n";

			GLbyte zeroes[4] = {0};
			glClearTexImage(this->surface->textureID, 0, GL_RGBA, GL_UNSIGNED_BYTE, zeroes);
			this->seed = seed;

			int coordsSetSize = 0;
			for (int i = scale; i >= 2; i/=2) {
				coordsSetSize += scale*scale / (i*i);
			}
			this->coords = new GLint[3 * coordsSetSize]();

			this->SetUpCoords(scale,0,0,scale);
			
		      	Textures t_coords(GL_TEXTURE_1D, coordsSetSize, 1, (GLvoid *) this->coords, GL_RGB, GL_INT, GL_RGBA32I);

			DSGL::ComputeProgram generativeSurface("GenerativeSurface.cs", DSGL_READ_FROM_FILE);
			
			glUseProgram(generativeSurface.ID);
			generativeSurface.Uniformui("scale", scale);
			//generativeSurface.Uniformui("depth", 8);
			//generativeSurface.Uniformui("brushesNumber", DSGL_GENERATIVE_VORONOI_CELLS);
			glUseProgram(0);


			this->surface->Bind(0);
			brushes->Bind(1);
			t_coords.Bind(2);
        		generativeSurface.Use(scale, scale,1);
			glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
			brushes->Unbind();
		}

		SquareSurface::~SquareSurface() {
			delete[] this->coords;
		}

		unsigned int SquareSurface::GetRandomFromSeed(unsigned int salt) {
		  	SquareSurface::seedOffset++;
			return ((unsigned int *) this->seed)[(salt+SquareSurface::seedOffset) % (4096 / sizeof(unsigned int))];
		}

		void SquareSurface::SetUpCoords(int scale, int offsetX, int offsetY, int realScale) {
        		if (scale == 64) {
				return;
			}

			int halfScale = scale >> 1;

			GLint randX = - halfScale + 1 + GetRandomFromSeed(offsetX - offsetY) % (scale-1);
			GLint randY = - halfScale + 1 + GetRandomFromSeed(offsetX + offsetY + randX) % (scale-1);
			
			GLint sign = GetRandomFromSeed(offsetX ^ offsetY) & 2 ? -1 : 1;

        		int offsetIndex = 0;
			
			for (int i = realScale; i > scale; i/=2) {
				offsetIndex += realScale*realScale / (i*i);
			}

			int localIndex = offsetX/scale + (realScale/scale) * offsetY/scale;
			
			this->coords[(offsetIndex + localIndex) * 3]		= randX;
			this->coords[(offsetIndex + localIndex) * 3 + 1 ]	= randY;
			this->coords[(offsetIndex + localIndex) * 3 + 2 ]	= sign;
			
			SetUpCoords(scale/2, offsetX+0, offsetY+0, realScale);
			SetUpCoords(scale/2, offsetX+0, offsetY+scale/2, realScale);
        		SetUpCoords(scale/2, offsetX+scale/2, offsetY+scale/2, realScale);
			SetUpCoords(scale/2, offsetX+scale/2, offsetY+0, realScale);
		}
	}
}
