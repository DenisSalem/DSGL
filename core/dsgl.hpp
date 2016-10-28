#include <cstdio>
#include <cstring>
#include <memory>
#include <iostream>
#include <fstream>

#if defined(DSGL_GL3W)
	#include <GL/gl3w.h>
#else
	/* INCLUDE ALTERNATIVE LIBRARY HERE */
#endif


#if defined(DSGL_GLFW)
	#include <GLFW/glfw3.h>
#endif

#ifndef DSGL_CORE_INCLUDED
#define DSGL_CORE_INCLUDED

#define DSGL_TRACE { std::cerr << "glError: " << glGetError() << ",\t" << __FILE__ << ", " << __LINE__ << "\n"; } 

#define DSGL_VERSION				"0.0.0"

#define DSGL_CLEAN_SHADERS_ONLY			true
#define DSGL_CLEAN_ALL				false

#define DSGL_FBO_NO_DEPTH			false
#define DSGL_FBO_DEPTH				true

#define DSGL_READ_FROM_STRING			1
#define DSGL_READ_FROM_FILE			0

#define DSGL_SHADER_ERROR_LENGTH		2048
#define DSGL_END_NICELY				0

#define DSGL_GLFW_INIT_FAILED			-1
#define DSGL_WINDOW_POINTER_NULL		-2
#define DSGL_GL3W_INIT_FAILED			-3
#define DSGL_CANNOT_READ_SHADER_SOURCE          -4
#define DSGL_ERROR_AT_SHDR_COMPILE_TIME         -5
#define DSGL_CANNOT_CREATE_SHADER		-6
#define DSGL_CANNOT_CREATE_PROGRAM		-7
#define DSGL_ID_DOESNT_NAME_A_PROGRAM	        -8
#define DSGL_VAO_DOESNT_EXIST			-9
#define DSGL_VBO_DOESNT_EXIST			-10
#define DSGL_IBO_DOESNT_EXIST			-11
#define DSGL_CANNOT_CREATE_VAO			-12
#define DSGL_CANNOT_CREATE_VBO			-13
#define DSGL_CANNOT_CREATE_IBO			-14
#define DSGL_VBO_IS_NULL			-15
#define DSGL_IBO_IS_NULL			-16
#define DSGL_INSTANCES_IS_NULL			-17
#define DSGL_INSTANCES_DOESNT_EXIST		-18
#define DSGL_UNIFORM_LOCATION_DOESNT_EXISTS	-19
#define DSGL_CANNOT_CREATE_FBO			-20
#define DSGL_CANNOT_CREATE_TEXTURE		-21
#define DSGL_CANNOT_CREATE_DEPTH_BUFFER		-22
#define DSGL_FBO_DOESNT_EXIST			-23

#define DSGL_MSG_GLFW_INIT_FAILED               "DSGL: GLFW initialization failed."
#define DSGL_MSG_GL3W_INIT_FAILED		"DSGL: GL3W initialization failed."
#define DSGL_MSG_WINDOW_POINTER_NULL            "DSGL: Window pointer is null." 
#define DSGL_MSG_VBO_IS_NULL			"DSGL: Vertex Buffer name is null."
#define DSGL_MSG_UNIFORM_LOCATION_DOESNT_EXISTS	"DSGL: Uniform location doesn't exist."
#define DSGL_MSG_CANNOT_CREATE_FBO		"DSGL: Cannot create frame buffer object."
#define DSGL_MSG_CANNOT_CREATE_VBO		"DSGL: VBO creation failed."
#define DSGL_MSG_CANNOT_CREATE_SHADER		"DSGL: Cannot create shader."
#define DSGL_MSG_CANNOT_CREATE_DEPTH_BUFFER	"DSGL: Cannot create depth buffer."
#define DSGL_MSG_ERROR_AT_SHDR_COMPILE_TIME	"DSGL: Cannot compile shader."
#define DSGL_MSG_CANNOT_CREATE_TEXTURE		"DSGL: Cannot create texture."
#define DSGL_MSG_VBO_DOESNT_EXIST		"DSGL: VBO doesn't exist."
#define DSGL_MSG_ID_DOESNT_NAME_A_PROGRAM	"DSGL: ID doesn't name a program."
#define DSGL_MSG_FBO_DOESNT_EXIST		"DSGL: FBO doesn't exist."

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

		#if defined(DSGL_GLFW)
			GLFWwindow * window;
		#else
			void * window;
		#endif
	};
	
	struct FrameBufferObject {
	  	FrameBufferObject(GLuint width, GLuint height);
	  	FrameBufferObject(GLuint width, GLuint height, bool option, GLenum buffertype);
	  	
		void DrawBind();
		void Bind();
		
		static void DrawUnbind();
		static void Unbind();
		
		GLuint ID;
		GLuint textureID;
		GLuint depthBufferID;
		GLuint width;
		GLuint height;
	};

	struct VertexBufferObject {
		VertexBufferObject(GLsizeiptr size, const GLvoid * data);
		VertexBufferObject(GLsizeiptr size, const GLvoid * data, GLenum usage);
		~VertexBufferObject();
		
		void Bind();

		static void Unbind();		
		
		GLuint ID;
	};

	struct Textures {
		Textures(GLuint target);
		Textures(GLuint target, GLuint width, GLuint height, GLvoid * rawData);

		void SetNormalMap(GLvoid * rawData);
		void Bind();
		void Unbind();

		GLuint textureID;
		int width;
		int height;
		GLvoid * rawData;
		GLuint normalMapID;
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

			
		void AttribPointer(GLuint index,GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer);
		void AttribPointer(GLuint buffer, GLuint index,GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer);
		void InstancesAttribPointer(GLuint index,GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer, GLuint divisor);
		
		GLuint ID;
		GLuint IBO;
		GLuint VBO;
		GLuint instances;
	};
	
	struct Elements {
		Elements(GLsizeiptr size,const GLvoid * data, GLenum usage);
		Elements(GLsizeiptr size,const GLvoid * data);
		
		~Elements();
		
		void Bind();
		
		GLuint ID;
	};
	
	class Shader {
		public:
			Shader(const char * inputShader, GLuint shaderType, int option);
			Shader(const char * inputShader, GLuint shaderType);

			~Shader();

			void ReadFromFile(const char * shaderFilename);
			
			std::string shaderSource;
			
			int shaderSourceSize;
			
			GLuint ID;
			GLint Result;
		private:
			char * shaderErrorMessages = NULL;

	};

	class ShaderProgram {
		public:
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

			void Uniformf(const char * uniformName, GLfloat v0);
			void Uniformf(const char * uniformName, GLfloat v0, GLfloat v1);
			void Uniformf(const char * uniformName, GLfloat v0, GLfloat v1, GLfloat v2);
				
			void Use();
	
			std::shared_ptr<Shader> vertex;
			std::shared_ptr<Shader> tesselationControl;
			std::shared_ptr<Shader> tesselationEvaluation;
			std::shared_ptr<Shader> geometry;
			std::shared_ptr<Shader> fragment;
				
			GLuint ID;
			GLint Result;

		private:
			char * programErrorMessages = NULL;
	};
}

#endif
