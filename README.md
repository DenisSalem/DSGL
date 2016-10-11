# DSGL 0.0.0

Damn Simple OpenGL

## About

DSGL is a tiny and simple modern OpenGL wrapper libraries written in C++ and intented to do not hide OpenGL but simply
make the programmer's life easier by providing some general purpose classes.

DSGL rely on [gl3w](https://github.com/skaslev/gl3w) to work. That mean's that by using DSGL you actually running [core profile](https://www.opengl.org/wiki/Core_And_Compatibility_in_Contexts) OpenGL.

## Installation and compilation

## API

Each classes and functions are defined within the DSGL namespace.

### Exception

	struct Exception {
		Exception(int code, const char * msg);
		Exception(int code, const char * msg, const char * filename);

		int code;
			
		std::string msg;
	
		std::string filename; 
	};
	
Where attributes

- __code__ may be one of the following:
 - *DSGL_GLFW_INIT_FAILED*
 - *DSGL_WINDOW_POINTER_NULL*
 - *DSGL_GL3W_INIT_FAILED*
 - *DSGL_CANNOT_READ_SHADER_SOURCE*
 - *DSGL_ERROR_AT_SHDR_COMPILE_TIME*
 - *DSGL_CANNOT_CREATE_SHADER*
 - *DSGL_CANNOT_CREATE_PROGRAM*
 - *DSGL_ID_DOESNT_NAME_A_PROGRAM*
 - *DSGL_VAO_DOESNT_EXIST*
 - *DSGL_VBO_DOESNT_EXIST*
 - *DSGL_IBO_DOESNT_EXIST*
 - *DSGL_CANNOT_CREATE_VAO*
 - *DSGL_CANNOT_CREATE_VBO*
 - *DSGL_CANNOT_CREATE_IBO*
- __msg__ is the message describing the error publicly available through member. If DSGL_DEGUG is defined Exception will print msg to stderr.
- __filename__ hold, when necessary, the filename related to the exception. For instance this is used to hold shader source filename.

Exception is thrown when error occurs while DSGL perform tasks.

### Context

	struct Context {
		Context(const char * name, int width, int height, int glMajorVersion, int glMinorVersion);
		Context(int width, int height, int glMajorVersion, int glMinorVersion);
			
		~Context();
			
		int InitSimpleWindow();

		int width;
		int height;
		int glMajorVersion;
		int glMinorVersion;
			
		std::string name;

		#ifdef DSGL_GLFW
			GLFWwindow * window;
		#endif
	};
	
Where attributes

- __name__ define the window title made with _InitSimpleWindow()_. Default name is an empty string.
- __width__ define horizontal length of the window made with _InitSimpleWindow()_.
- __height__ define vertical length of the window made with _InitSimpleWindow()_.
- __glMajorVersion__ tell wich major version of OpenGL you want to use.
- __glMinorVersion__ tell wich minor version of OpenGL you want to use.
- __window__ window hold the window pointer defined by the API you want to use. For instance only GLFW is available.

and where methods

- __InitSimpleWindow()__ make current context active and create simple window.


### VertexBufferObject

	struct VertexBufferObject {
		VertexBufferObject(GLsizeiptr size, const GLvoid * data);
		VertexBufferObject(GLsizeiptr size, const GLvoid * data, GLenum usage);
		~VertexBufferObject();
		
		void Bind();
		static void Unbind();		
		
		GLuint ID = 0;
	};

where attributes

- __ID__ name the VBO created with VertexBufferObject constructor.

and where methods

- __VertexBufferObject(GLsizeiptr size, const GLvoid * data)__ create a VBO fed with _data_ with a size defined by _size_. By default VBO is setup with _GL_STATIC_DRAW_.
- __VertexBufferObject(GLsizeiptr size, const GLvoid * data, GLenum usage)__ Same as above but setup VBO usage with _usage_, where _usage_ may be one of the following ([]see glBufferData man page](https://www.opengl.org/sdk/docs/man4/html/glBufferData.xhtml)):
 - _GL_STREAM_DRAW_
 - _GL_STREAM_READ_
 - _GL_STREAM_COPY_
 - _GL_STATIC_DRAW_
 - _GL_STATIC_READ_
 - _GL_STATIC_COPY_
 - _GL_DYNAMIC_DRAW_
 - _GL_DYNAMIC_READ_
 - _GL_DYNAMIC_COPY_
- __Bind()__ make the VBO active.
- __Unbind__ unbinds any buffer object previously bound.

### VertexArrayObject

