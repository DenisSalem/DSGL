#include "dsgl.hpp"
#include "dsglMeshes.hpp"

#include <unistd.h>

#include <glm/glm.hpp>

int main() {
	/* Init Context and create shader program used to render our instances. */

	DSGL::Context context("INSTANCING QUADS WITH DSGL", 640, 480, 3, 3);
	
	context.InitSimpleWindow();
	
	DSGL::PipelineProgram pipelineProgram("vertex.shader","fragment.shader");

	/* Define our mesh, a quad for instance. */
	DSGL::Meshes::Quad quad;

	/* Define instances */
	glm::vec2 translations[100];
	int index = 0;
	GLfloat offset = 0.1f;
	for(GLint y = -10; y < 10; y += 2) {
		for(GLint x = -10; x < 10; x += 2) {
			glm::vec2 translation;
			translation.x = (GLfloat) x / 10.0f + offset;
			translation.y = (GLfloat) y / 10.0f + offset;
			translations[index++] = translation;
		}
	}  

	/* Create buffers */
	DSGL::Elements elements(4 * sizeof(GLuint), quad.index);
	DSGL::VertexBufferObject VBO(sizeof(GLfloat) * 12, quad.vertex);
	DSGL::VertexBufferObject instances(sizeof(glm::vec2) * 100, translations);
	DSGL::VertexArrayObject VAO(elements.ID, VBO.ID, instances.ID);
	
	/* Set up how vertex related memory is organized */
	VAO.AttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	/* Set up how instances memory is organized */
	VAO.InstancesAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0, 1);

	/* Actual render loop, for instance rely on glfw */
	glEnable(GL_CULL_FACE);
	
	while (!glfwWindowShouldClose(context.window)) {
		glfwPollEvents();
		
		glfwSwapBuffers(context.window);
				
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		pipelineProgram.Use(); /* Optional since we're using only one shaderProgram, but must be called at least one time */
		
		usleep(40000); /* Let the GPU take a breath */

		VAO.Bind(); /* Optional since we're using only one VAO, but must be called at least one time */

		glDrawElementsInstanced(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (GLvoid *) 0, 100);

		glBindBuffer(GL_ARRAY_BUFFER, 0); /* Optional since we're using only one VAO */

		glUseProgram(0); /* Optional since we're using only one shader program */

	}

	return DSGL_END_NICELY;
}
