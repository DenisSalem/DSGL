#ifndef DSGL
#define DSGL
#include <cstring>
#include <GL/gl3w.h>

#ifdef DSGL_GLFW
#include <GLFW/glfw3.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define DSGL_GLFW_INIT_FAILED		-1
#define DSGL_WINDOW_POINTER_NULL	-2
#define DSGL_GLFW_INIT_FAILED		-3

namespace DSGL {
	void PrintWorkGroupsCapabilities();
	
	class Context {
		public:
			Context(const char * name, int width, int height, int glMajorVersion, int glMinorVersion);
			Context(const char * name, int width, int height);
			~Context();
			void InitSimpleWindow();

			int width;
			int height;
			int glMajorVersion;
			int glMinorVersion;
			
			char * name;

			#ifdef DSGL_GLFW
			GLFWwindow * window;
			#endif
		private:
			void init(const char * name, int width, int height, int glMajorVersion, int glMinorVersion);
	}
	
	class Shader {
		public:
			DSGLShader(const char * shaderFilename);
			DSGLShader(const char * shaderFilename, bool verbose);

			~DSGLShader();

			void SetVerbose(bool verbose);
			void EchoErrors();
			GLuint GetID();
			
			char * shaderSource;
			char * shaderErrorMessages;
	}

	class ShaderProgram {
	
	}
}

#endif