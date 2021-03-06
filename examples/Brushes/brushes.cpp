#include <iostream>
#include <unistd.h>
#include <random>
#include <chrono>
#include "dsgl.hpp"
#include "dsglMeshes.hpp"
#include "dsglBrushes.hpp"

int main(int argc, char ** argv) {
	try {
		/* OpenGL context */
		DSGL::Context context("GENERATIVE SURFACE WITH DSGL", 512 * DSGL_GENERATIVE_BRUSHES_MAX_NUMBER, 512, 4, 3);
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
		
		auto time = std::chrono::high_resolution_clock::now();
                auto count = time.time_since_epoch().count();

		std::default_random_engine generator;
		std::uniform_int_distribution<int> distribution(0,4294967295);
                generator.seed(std::default_random_engine::result_type(count));
		
		auto dice = std::bind ( distribution, generator );
		
		char * seed = new char[4096];
		for (int i = 0; i < 4096 / sizeof(unsigned int); i++) {
			((unsigned int*)seed)[i] = (unsigned int) dice();
		}

		std::chrono::high_resolution_clock::time_point t1,t2;
		t1 = std::chrono::high_resolution_clock::now(); 
		
		DSGL::Generative::Brushes brush(512, seed);
		glFinish();
		t2 = std::chrono::high_resolution_clock::now();	
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
		std::cout << "Duration: " << float(duration) / 1000000.0 << "s\n";
		
		DSGL_TRACE
		// ----- Render loop ----- //
		while (!glfwWindowShouldClose(context.window)) {
			glfwPollEvents();
		
			usleep(40000); // Let the GPU take a breath //
    		
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// ----- Actual render ----- //
    
			pipelineProgram.Use();
    
			VAO.Bind();
    
			brush.brushes->Bind();
    
			glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (GLvoid *) 0);
    
			brush.brushes->Unbind();
            
			glBindVertexArray(0);
    
			glUseProgram(0);

			glfwSwapBuffers(context.window);
		}

		delete[] seed;
	}
	catch (DSGL::Exception &e) {
		std::cerr << e.msg << "\n";
	}


	return 0;
}
