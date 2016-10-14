#include "dsgl.hpp"

#include <sstream>
#include <unistd.h>

#include <glm/glm.hpp>

int main() {

	/* Init Context and create shader program used to render our instances. */

	DSGL::Context context("DSGL SIMPLE WINDOW PROGRAM",640,480,4,3);
	
	context.InitSimpleWindow();
	
	DSGL::ShaderProgram shaderProgram("vertex.shader","fragment.shader");

	/* Define our mesh, a quad for instance. */

	unsigned short int quadIndex[4] = {0,1,2,3};

	GLfloat quadVertices[] = {
		// Positions
		-0.05f,  0.05f,
		-0.05f, -0.05f,
		 0.05f,  0.05f, 	
		 0.05f, -0.05f
	};  
	
	/* Define instances */

	glm::vec2 translations[100];
	int index = 0;
	GLfloat offset = 0.1f;
	for(GLint y = -10; y < 10; y += 2){
		for(GLint x = -10; x < 10; x += 2) {
			glm::vec2 translation;
			translation.x = (GLfloat)x / 10.0f + offset;
			translation.y = (GLfloat)y / 10.0f + offset;
			translations[index++] = translation;
		}
	}  

	/* Setup our buffers */

	DSGL::Elements elements(4 * sizeof(unsigned short int), quadIndex);
	DSGL::VertexBufferObject VBO(sizeof(GLfloat) * 8, quadVertices);
	DSGL::VertexBufferObject instances(sizeof(glm::vec2) * 100, translations);

	DSGL::VertexArrayObject VAO(elements.ID, VBO.ID, instances.ID);
	
	/* Set up how vertex related memory is organized */
	VAO.AttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);

	/* Set up how instances memory is organized */
	VAO.InstancesAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0, 1);

	glEnable(GL_CULL_FACE);
	
	/* Actual render loop, for instance rely on glfw */

	while (!glfwWindowShouldClose(context.window)) {
        	glfwSwapBuffers(context.window);
        	
		glfwPollEvents();
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		shaderProgram.Use(); /* Optional since we're using only one shaderProgram, but must be called at least one time */

		VAO.Bind(); /* Optional since we're using only one VAO, but must be called at least one time */

		usleep(40000); /* Let the GPU take a breath */

		glDrawElementsInstanced(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, (GLvoid *) 0, 100);

		VAO.Unbind(); /* Optional since we're using only one VAO */

		glUseProgram(0); /* Optional since we're using only one shader program */

		glfwSwapBuffers(context.window);
	}

	return DSGL_END_NICELY;
}
