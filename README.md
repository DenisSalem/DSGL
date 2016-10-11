# DSGL

Damn Simple OpenGL

## About

DSGL is a tiny and simple modern OpenGL wrapper libraries written in C++ and intented to do not hide OpenGL but simply
make the programmer's life easier by providing some general purpose classes.

## Installation and compilation

## API

Each classes and functions are defined within the DSGL namespace.

### Exception

If DSGL_DEGUG is defined Exception will produce verbose output related to the current error.

_struct Exception {
		Exception(int code, const char * msg);
		Exception(int code, const char * msg, const char * filename);

		int code;
			
		std::string msg;
		std::string filename; 
};_
	
Where

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
- __msg__ is the message describing the error