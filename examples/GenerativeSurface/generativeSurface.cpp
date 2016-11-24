#include <iostream>
#include <unistd.h>
#include <random>
#include <chrono>
#include "dsgl.hpp"
#include "dsglMeshes.hpp"
#include "dsglBrushes.hpp"
#include "dsglGenerative.hpp"
#ifdef __i386

extern __inline__ uint64_t rdtsc(void) {
  uint64_t x;
  __asm__ volatile ("rdtsc" : "=A" (x));
  return x;
}
#elif defined __amd64
extern __inline__ uint64_t rdtsc(void) {
  uint64_t a, d;
  __asm__ volatile ("rdtsc" : "=a" (a), "=d" (d));
  return (d<<32) | a;
}
#endif

int main(int argc, char ** argv) {
	try {
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
		
		t2 = std::chrono::high_resolution_clock::now();	
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
    		std::cout << "Brushes duration: " << float(duration) / 1000000.0 << "s\n";
		
		t1 = std::chrono::high_resolution_clock::now();	
		
		DSGL::Generative::SquareSurface surface(512, brush.brushes, seed);
		
		t2 = std::chrono::high_resolution_clock::now();	
		duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
    		std::cout << "Surface duration: " << float(duration) / 1000000.0 << "s\n";

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
	}
	catch (DSGL::Exception &e) {
		std::cerr << e.msg << "\n";
	}


	return 0;
}
