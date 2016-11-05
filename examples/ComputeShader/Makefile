make:
	rm computeShader -f
	make computeShader

computeShader:
	g++ main.cpp dsgl.cpp dsglMeshes.cpp gl3w.c -o computeShader -lglfw -lGL -ldl -D DSGL_DEBUG -D DSGL_GLFW -D DSGL_GL3W -std=c++11
