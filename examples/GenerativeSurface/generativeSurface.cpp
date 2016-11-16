#include <iostream>
#include <unistd.h>
#include "dsgl.hpp"
#include "dsglMeshes.hpp"
#include "dsglBrushes.hpp"
#include "dsglGenerative.hpp"

int main(int argc, char ** argv) {
	/* OpenGL context */
	DSGL::Context context("CUBEMAPING WITH DSGL", 512, 512, 4, 4);
	context.InitSimpleWindow();
	
	/* Regular and compute shaders */
	DSGL::PipelineProgram pipelineProgram("vertex.shader","fragment.shader");

	/* Create buffers */
	DSGL::Meshes::Quad quad;

	DSGL::Elements elements(4 * sizeof(GLuint), quad.index);
	DSGL::VertexBufferObject VBO(sizeof(GLfloat) * 12, quad.vertex);
	DSGL::VertexBufferObject texCoords(sizeof(GLfloat) * 8, quad.texCoords);
	DSGL::VertexArrayObject VAO(elements.ID, VBO.ID);
	
	/* Set up how vertex related memory is organized */
	VAO.AttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	/* Set up how texcoords is organized in memory */
  	VAO.AttribPointer(texCoords.ID, 1, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), 0);
	
	char * seed = new char[4096];

	for (int i = 0; i < 4096 / sizeof(unsigned long int); i++) {
		((unsigned long int *) seed)[i] = DSGL::GetRandom(i);
	}

	DSGL::Generative::Brushes brush(512, seed);
	
	DSGL::Generative::SquareSurface surface(512, brush.brushes, seed);
	
	// ----- Render loop ----- //
	while (!glfwWindowShouldClose(context.window)) {
		glfwPollEvents();
		
		usleep(40000); // Let the GPU take a breath //
    		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// ----- Actual render ----- //
    
		pipelineProgram.Use();
    
		VAO.Bind();
    
		surface.surface->Bind();
    
		glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (GLvoid *) 0);
    
		surface.surface->Unbind();
            
		glBindVertexArray(0);
    
		glUseProgram(0);

		glfwSwapBuffers(context.window);
	}

	delete[] seed;

	return 0;
}
