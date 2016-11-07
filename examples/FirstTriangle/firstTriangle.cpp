#include "dsgl.hpp"
#include "dsglMeshes.hpp"
#include <unistd.h>

int main() {
	/* Init Context and create shader program used to render our instances. */

	DSGL::Context context("DSGL HELLO TRIANGLE",640,480,4,3);
	
	context.InitSimpleWindow();
	
	DSGL::PipelineProgram pipelineProgram("vertex.shader","fragment.shader");

	/* DSGL provide built in simple triangle for playing and debugging. */
	DSGL::Meshes::Triangle triangle;

	/* Create buffers */
	DSGL::Elements elements(3 * sizeof(GLuint), triangle.index);
	DSGL::VertexBufferObject VBO(sizeof(GLfloat) * 9, triangle.vertex);
	DSGL::VertexArrayObject VAO(elements.ID, VBO.ID);
	
	/* Set up how vertex related memory is organized */
	VAO.AttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	glEnable(GL_CULL_FACE);
	
	/* Actual render loop, for instance rely on glfw */

	while (!glfwWindowShouldClose(context.window)) {
        	glfwSwapBuffers(context.window);
        	
		glfwPollEvents();
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		pipelineProgram.Use(); /* Optional since we're using only one shaderProgram, but must be called at least one time */

		VAO.Bind(); /* Optional since we're using only one VAO, but must be called at least one time */

		usleep(40000); /* Let the GPU take a breath */

		glDrawElements(GL_TRIANGLE_STRIP, 3, GL_UNSIGNED_INT, (GLvoid *) 0);

		glBindBuffer(GL_ARRAY_BUFFER, 0); /* Optional since we're using only one VAO */

		glUseProgram(0); /* Optional since we're using only one shader program */
	}

	return DSGL_END_NICELY;
}
