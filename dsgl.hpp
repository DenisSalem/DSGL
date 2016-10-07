#include <cstdio>
#include <cstring>
#include <iostream>
#include <fstream>
#include <GL/gl3w.h>

#ifdef DSGL_GLFW
#include <GLFW/glfw3.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define DSGL_VERSION					"0.0.0"

#define DSGL_CLEAN_SHADER_ONLY			true
#define DSGL_CLEAN_ALL					false

#define DSGL_READ_FROM_STRING			1
#define DSGL_READ_FROM_FILE				0

#define DSGL_SHADER_ERROR_LENGTH		2048
#define DSGL_END_NICELY					0

#define DSGL_GLFW_INIT_FAILED			-1
#define DSGL_WINDOW_POINTER_NULL		-2
#define DSGL_GL3W_INIT_FAILED			-3
#define DSGL_CANNOT_READ_SHADER_SOURCE  -4
#define DSGL_ERROR_AT_SHDR_COMPILE_TIME -5
#define DSGL_CANNOT_CREATE_SHADER		-6
#define DSGL_CANNOT_CREATE_PROGRAM		-7


namespace DSGL {
	int GetFileSize(const char * inputFilePath);
	void PrintNicelyWorkGroupsCapabilities();
	
	class Context {
		public:
			Context(const char * name, int width, int height, int glMajorVersion, int glMinorVersion);
			Context(int width, int height, int glMajorVersion, int glMinorVersion);
			
			~Context();
			
			int InitSimpleWindow();

			int width;
			int height;
			int glMajorVersion;
			int glMinorVersion;
			
			char * name;

			#ifdef DSGL_GLFW
				GLFWwindow * window;
			#endif
			
		private:
			void Init(const char * name, int width, int height, int glMajorVersion, int glMinorVersion);
	};
	
	class Shader {
		public:
			Shader(const char * inputShader, GLuint shaderType);
			Shader(const char * inputShader, GLuint shaderType, int option);

			~Shader();

			void ReadFromFile(const char * shaderFilename);
			
			char * shaderSource = NULL;
			char * shaderErrorMessages = NULL;
			
			int shaderSourceSize;
			
			GLuint ID;
			GLint Result;
			
		private:
			int Init(const char * inputShader, GLuint shaderType, int option);
	};

	class ShaderProgram {
		public:
				ShaderProgram();
				ShaderProgram(const char * inputVertexShader, const char * inputFragmentShader);
				ShaderProgram(
					const char * inputVertexShader,
					const char * inputTesselationControlShader,
					const char * inputTesselationEvaluationShader,
					const char * inputGeometryShader,
					const char * inputFragmentShader
				);
				
				~ShaderProgram();
				
				void Clean(bool shadersOnly);
				
				char * programErrorMessages = NULL;

				Shader * vertex = NULL;
				Shader * tesselationControl = NULL;
				Shader * tesselationEvaluation = NULL;
				Shader * geometry = NULL;
				Shader * fragment = NULL;
				
				GLuint ID;
				GLint Result;

		private:
			int Init(
				const char * inputVertexShader,
				const char * inputTesselationControlShader,
				const char * inputTesselationEvaluationShader,
				const char * inputGeometryShader,
				const char * inputFragmentShader
			);
	};
}

