#include "dsgl.hpp"
#include <sstream>
#include <unistd.h>

int main() {
	DSGL::Context context("DSGL SIMPLE WINDOW PROGRAM",640,480,4,3);
	
	context.InitSimpleWindow();
	
	DSGL::ShaderProgram shaderProgram("vertex.shader","fragment.shader");

	unsigned short int quadIndex[4] = {0,1,2,3};

	GLfloat quadVertices[] = {
		// Positions
		-0.05f,  0.05f,
		-0.05f, -0.05f,
		 0.05f,  0.05f, 	
		 0.05f, -0.05f
	};  
	
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

	DSGL::Elements elements(4 * sizeof(unsigned short int), quadIndex);
	
	DSGL::VertexBufferObject VBO(sizeof(GLfloat) * 8, quadVertices);
	DSGL::VertexBufferObject instances(sizeof(glm::vec2) * 100, translations);
	
	DSGL::VertexArrayObject VAO(elements.ID, VBO.ID, instances.ID);
	
	VAO.AttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
	VAO.InstancesAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0, 1);

    glEnable(GL_CULL_FACE);
	glEnable (GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
	while (!glfwWindowShouldClose(context.window))
    {
        glfwSwapBuffers(context.window);
        glfwPollEvents();
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		shaderProgram.Use();

		VAO.Bind();

		usleep(40000);

		glDrawElementsInstanced(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, (GLvoid *) 0, 100);

		VAO.Unbind();
		glUseProgram(0);

		glfwSwapBuffers(context.window);
    }

	return 0;
}