#include <iostream>
#include <unistd.h>
#include "dsgl.hpp"
#include "dsglMeshes.hpp"
#include "dsglPng.hpp"

int main(int argc, char ** argv) {
	if (argc != 2) {
	  	std::cout << "Usage: ./texturePng <myImage.png>\n";
		return 0;
	}

	DSGL::Images::Png png(argv[1]);

	std::cout << "ColorType: " << png.ColorType() << "\n";
	std::cout << "Width: " << png.Width() << "\n";
	std::cout << "Height: " << png.Height() << "\n";
	std::cout << "Format: " << png.image.format << "\n";
	std::cout << "Interlace: " <<  (int) (((DSGL::Images::IHDRCHUNK *) (png.pngStruct.IHDR.Data))->BitDepth) << "\n";
	std::cout << "Channel size: " << PNG_IMAGE_SAMPLE_CHANNELS(png.image.format) << "\n";

	/* OpenGL context */

	DSGL::Context context("OPEN AND DISPLAY IMAGE WITH DSGL", png.Width(), png.Height(), 3, 3);
	context.InitSimpleWindow();
	
	/* Regular shaders */
	DSGL::PipelineProgram pipelineProgram("vertex.shader","fragment.shader");

	/* DSGL provide built in simple quad for playing with and debugging. */
	DSGL::Meshes::Quad quad;

	/* Create buffers */
	DSGL::Texture texture(GL_TEXTURE_2D, png.Width(), png.Height(), png.rawData, png.GetFormat(), png.GetType());
	DSGL::Elements elements(4 * sizeof(GLuint), quad.index);
	DSGL::VertexBufferObject VBO(sizeof(GLfloat) * 12, quad.vertex);
	DSGL::VertexBufferObject texCoords(sizeof(GLfloat) * 8, quad.texCoords);
	DSGL::VertexArrayObject VAO(elements.ID, VBO.ID);

	/* Set up how vertex related memory is organized */
	VAO.AttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	/* Set up how texcoords is organized in memory */
  	VAO.AttribPointer(texCoords.ID, 1, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), 0);
	glEnable (GL_BLEND); glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	/* ----- Render loop ----- */
	while(!glfwWindowShouldClose(context.window)) {
		glfwSwapBuffers(context.window);

		glfwPollEvents();

    		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/* ----- Actual render ----- */
    
		pipelineProgram.Use();
    
		VAO.Bind();
    
		texture.Bind();
    
		glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (GLvoid *) 0);
		
		usleep(40000); /* Let the GPU take a breath */
    
		texture.Unbind();
            
		glBindVertexArray(0);
    
		glUseProgram(0);
	}

	return DSGL_END_NICELY;
}
