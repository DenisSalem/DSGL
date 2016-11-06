#include <iostream>

#include "dsgl.hpp"
#include "dsglMeshes.hpp"
#include "dsglCubemap.hpp"

int main(int argc, char ** argv) {
	/* OpenGL context */
	DSGL::Context context("CUBEMAPING WITH DSGL", 256, 256, 4, 3);
	context.InitSimpleWindow();
	
	/* Regular and compute shaders */
	DSGL::PipelineProgram pipelineProgram("vertex.shader","fragment.shader");
	DSGL::ComputeProgram bell("bell.cs");
	glUseProgram(bell.ID);
	bell.Uniformui("brushScale", 256);
	glUseProgram(0);

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
	
	DSGL::Cubemap::Brushes brush(256);

	/* ----- Render loop ----- */
	while(true) {
		glEnable(GL_CULL_FACE);
    		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/* ----- Actual render ----- */
    
		pipelineProgram.Use();
    
		VAO.Bind();
    
		brush.brushes->Bind();
    
		glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (GLvoid *) 0);
    
		brush.brushes->Unbind();
            
		glBindVertexArray(0);
    
		glUseProgram(0);

		glfwSwapBuffers(context.window);
	}

	return 0;
}
