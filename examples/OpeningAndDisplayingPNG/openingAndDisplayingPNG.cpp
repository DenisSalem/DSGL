#include <iostream>
#include "dsgl.hpp"
#include "dsglMeshes.hpp"
#include "dsglPng.hpp"

int main(int argc, char ** argv) {
	if (argc != 2) {
	  	std::cout << "Usage: ./readPng <myImage.png>\n";
		return 0;
	}

	DSGL::Images::Png png(argv[1]);

	/* OpenGL context */
	DSGL::Context context("DAMN SIMPLE COMPUTE SHADER WITH DSGL", png.Width(), png.Height(), 4, 3);
	context.InitSimpleWindow();
	
	/* Regular and compute shaders */
	DSGL::PipelineProgram pipelineProgram("vertex.shader","fragment.shader");

	/* DSGL provide built in simple quad for playing with and debugging. */
	DSGL::Meshes::Quad quad;

	/* Create buffers */
	DSGL::Textures texture(GL_TEXTURE_2D, 640,480, NULL);
	DSGL::Elements elements(4 * sizeof(GLuint), quad.index);
	DSGL::VertexBufferObject VBO(sizeof(GLfloat) * 12, quad.vertex);
	DSGL::VertexBufferObject texCoords(sizeof(GLfloat) * 8, quad.texCoords);
	DSGL::VertexArrayObject VAO(elements.ID, VBO.ID);

	/* Set up how vertex related memory is organized */
	VAO.AttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	/* Set up how texcoords is organized in memory */
  	VAO.AttribPointer(texCoords.ID, 1, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), 0);
	
	/* ----- Render loop ----- */
	while(!glfwWindowShouldClose(context.window)) {
		glEnable(GL_CULL_FACE);
    		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/* ----- Actual render ----- */
    
		pipelineProgram.Use();
    
		VAO.Bind();
    
		texture.Bind();
    
		glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (GLvoid *) 0);
    
		texture.Unbind();
            
		glBindVertexArray(0);
    
		glUseProgram(0);

		glfwSwapBuffers(context.window);
	}

	return 0;
}
