# ![DSGL](https://framagit.org/denissalem/DSGL/raw/a0abd277147e41a7e0846b84d4a14dfed1a3e8a6/doc/dsglLogo.png "DSGL")

# MESHES

To use DSGL MESHES features just include [meshes/dsglMeshes.hpp](https://framagit.org/denissalem/DSGL/blob/master/meshes/dsglMeshes.hpp) and [meshes/dsglMeshes.cpp](https://framagit.org/denissalem/DSGL/blob/master/meshes/dsglMeshes.cpp) in your project.

Every classes there are defined within DSGL::Meshes namespace.

- [Triangle](#triangle)
- [Quad](#quad)

## Triangle

		struct Triangle {
		  	Triangle();
			Triangle(GLfloat x, GLfloat y, GLfloat z);

			GLfloat		vertex[9];
			GLuint		index[3];
		};

where attributes

- __vertex__ hold vertex data. three vertex as a tuple of three GLfloat.
- __index__ hold index for elements buffer. Three for each vertex.

and methods

- __Triangle__ setup by default a wide screen triangle. x,y,z may be defined and act as offset. 

Example of usage belong there: [FirstTriangle](https://framagit.org/denissalem/DSGL/tree/master/examples/FirstTriangle).

## Quad

		struct Quad {
		  	Quad();
			Quad(GLfloat x, GLfloat y, GLfloat z);
			
			GLfloat		vertex[12];
			GLfloat		texCoords[8];
			GLuint		index[4];
		};

where attributes

- __vertex__ hold vertex data. four vertex as a tuple of three GLfloat.
- __texCoords__ hold texture coordinates as a tuple of two GLfloat. Four tuples for each vertex.
- __index__ hold index for elements buffer. Four for each vertex.

and methods

- __Quad__ setup by default a wide screen Quad. x,y,z may be defined and act as offset.

There is multiple examples of usage there:

- [ComputeShader](https://framagit.org/denissalem/DSGL/tree/master/examples/ComputeShader)
- [Instancing](https://framagit.org/denissalem/DSGL/tree/master/examples/Instancing)
- [TexturePNG](https://framagit.org/denissalem/DSGL/tree/master/examples/TexturePNG)
