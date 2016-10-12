#include <cstdio>
#include <cstring>
#include <memory>
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
#define DSGL_ID_DOESNT_NAME_A_PROGRAM	-8
#define DSGL_VAO_DOESNT_EXIST			-9
#define DSGL_VBO_DOESNT_EXIST			-10
#define DSGL_IBO_DOESNT_EXIST			-11
#define DSGL_CANNOT_CREATE_VAO			-12
#define DSGL_CANNOT_CREATE_VBO			-13
#define DSGL_CANNOT_CREATE_IBO			-14
#define DSGL_VBO_IS_NULL				-15
#define DSGL_IBO_IS_NULL				-16
#define DSGL_INSTANCES_IS_NULL			-17
#define DSGL_INSTANCES_DOESNT_EXIST		-18

#define DSGL_MSG_VBO_IS_NULL			"DSGL: Vertex Buffer name is null."

namespace DSGL {
	int GetFileSize(const char * inputFilePath);
	void PrintNicelyWorkGroupsCapabilities();
	
	struct Exception {
			Exception(int code, const char * msg);
			Exception(int code, const char * msg, const char * filename);

			int code;
			
			std::string msg;
			std::string filename; 
	};
	
	struct Context {
		Context(const char * name, int width, int height, int glMajorVersion, int glMinorVersion);
		Context(int width, int height, int glMajorVersion, int glMinorVersion);
			
		~Context();
			
		void InitSimpleWindow();

		int width;
		int height;
		int glMajorVersion;
		int glMinorVersion;
			
		std::string name;

		#ifdef DSGL_GLFW
			GLFWwindow * window;
		#endif
	};
	
	struct VertexBufferObject {
		VertexBufferObject(GLsizeiptr size, const GLvoid * data);
		VertexBufferObject(GLsizeiptr size, const GLvoid * data, GLenum usage);
		~VertexBufferObject();
		
		void Bind();

		static void Unbind();		
		
		GLuint ID = 0;
	};
	
	struct VertexArrayObject {
		VertexArrayObject();
		VertexArrayObject(GLuint IBO, GLuint VBO);
		VertexArrayObject(GLuint IBO, GLuint VBO, GLuint instances);
			
		~VertexArrayObject();
			
		void Bind();
		void SetVertex(GLuint vertex);
		void SetElements(GLuint elements);
		void SetInstances(GLuint instance);

		static void Unbind();
			
		void AttribPointer(GLuint index,GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer);
		void InstancesAttribPointer(GLuint index,GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer, GLuint divisor);
		
		GLuint ID	= 0;
		GLuint IBO	= 0;
		GLuint VBO	= 0;
		GLuint instances = 0;
	};
	
	struct Elements {
		Elements(GLsizeiptr size,const GLvoid * data, GLenum usage);
		Elements(GLsizeiptr size,const GLvoid * data);
		
		~Elements();
		
		void Bind();
		static void Unbind();
		
		GLuint ID	= 0;
	};
	
	class Shader {
		public:
			Shader(const char * inputShader, GLuint shaderType, int option);
			Shader(const char * inputShader, GLuint shaderType);

			~Shader();

			void ReadFromFile(const char * shaderFilename);
			
			std::string shaderSource;
			
			int shaderSourceSize;
			
			GLuint ID = 0;
			GLint Result;
		private:
			char * shaderErrorMessages = NULL;

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
				
			void Use();
	
			std::shared_ptr<Shader> vertex;
			std::shared_ptr<Shader> tesselationControl;
			std::shared_ptr<Shader> tesselationEvaluation;
			std::shared_ptr<Shader> geometry;
			std::shared_ptr<Shader> fragment;
				
			GLuint ID = 0;
			GLint Result;
		private:
			char * programErrorMessages = NULL;
	};
}

