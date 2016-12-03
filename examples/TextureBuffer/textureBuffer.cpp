#include <iostream>
#include <unistd.h>
#include "dsgl.hpp"
#include "dsglMeshes.hpp"

int main(int argc, char ** argv) {

	// Setup chess board //
	GLuint * texData = new GLuint[640 * 480 * 4]();
	for (int i = 0; i < 640; i++) {
		for (int j = 0; j < 480; j++) {
			if ((i/16 + j/16 ) % 2 == 0) {
				texData[(i + j * 640) * 4]	= 4294967295;
				texData[(i + j * 640) * 4 + 1]	= 2147483647;
				texData[(i + j * 640) * 4 + 2]	= 0;
				texData[(i + j * 640) * 4 + 3]	= 4294967295;
			}
			else  {
				texData[(i + j * 640) * 4]	= 0;
				texData[(i + j * 640) * 4 + 1]	= 2147483647;
				texData[(i + j * 640) * 4 + 2]	= 4294967295;
				texData[(i + j * 640) * 4 + 3]	= 4294967295;
			}
		}
	}

	// OpenGL context //

	DSGL::Context context("TEXTURE BUFFER DEMO WITH DSGL", 640, 480, 3, 3);
	context.InitSimpleWindow();
	
	// Regular shaders //
	DSGL::PipelineProgram pipelineProgram("vertex.shader","fragment.shader");

	// DSGL provide built in simple quad for playing with and debugging. //
	DSGL::Meshes::Quad quad;

	// Create buffers //
	DSGL::TextureBuffer texture(640*480*4*sizeof(GLuint), texData, GL_STATIC_DRAW, GL_RGBA32UI);
	DSGL::Elements elements(4 * sizeof(GLuint), quad.index);
	DSGL::VertexBufferObject VBO(sizeof(GLfloat) * 12, quad.vertex);
	DSGL::VertexBufferObject texCoords(sizeof(GLfloat) * 8, quad.texCoords);
	DSGL::VertexArrayObject VAO(elements.ID, VBO.ID);

	// Set up how vertex related memory is organized //
	VAO.AttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Set up how texcoords is organized in memory //
  	VAO.AttribPointer(texCoords.ID, 1, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), 0);

	// Set up how vertex related memory is organized //
	VAO.AttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Set up how texcoords is organized in memory //
  	VAO.AttribPointer(texCoords.ID, 1, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), 0);
	
	// ----- Render loop ----- //
	while(true) {
		glEnable(GL_CULL_FACE);
    		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// ----- Actual render ----- //
    
		pipelineProgram.Use();
    
		VAO.Bind();
    
		texture.Bind();
    
		glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (GLvoid *) 0);
    
		texture.Unbind();
            
		glBindVertexArray(0);
    
		glUseProgram(0);

		glfwSwapBuffers(context.window);
	}


	delete[] texData;

	return 0;
}
