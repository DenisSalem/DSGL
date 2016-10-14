# CORE

To use DSGL core features just include [dsgl.hpp](https://github.com/DenisSalem/DSGL/blob/master/core/dsgl.hpp) and [dsgl.cpp](https://github.com/DenisSalem/DSGL/blob/master/core/dsgl.cpp) in your project.

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
 - *DSGL_VBO_IS_NULL*
 - *DSGL_IBO_IS_NULL*
 - *DSGL_INSTANCES_IS_NULL*
 - *DSGL_INSTANCES_DOESNT_EXIST*
- __msg__ is the message describing the error publicly available through member. If DSGL_DEGUG is defined Exception will print msg to stderr.
- __filename__ hold, when necessary, the filename related to the exception. For instance this is used to hold shader source filename.

Exception is thrown when error occurs while DSGL perform tasks.

### Context

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
	
Where attributes

- __name__ define the window title made with _InitSimpleWindow()_. Default name is an empty string.
- __width__ define horizontal length of the window made with _InitSimpleWindow()_.
- __height__ define vertical length of the window made with _InitSimpleWindow()_.
- __glMajorVersion__ tell wich major version of OpenGL you want to use.
- __glMinorVersion__ tell wich minor version of OpenGL you want to use.
- __window__ window hold the window pointer defined by the API you want to use. For instance only GLFW is available.

and where methods

- __InitSimpleWindow()__ make current context active and create simple window. May issue exception with code *DSGL_GLFW_INIT_FAILED*, *DSGL_WINDOW_POINTER_NULL* and *DSGL_GL3W_INIT_FAILED*.
- Constructor and deleguates constructors do nothing more than setup object attributes.
- Default destructor wipe out the whole OpenGL context.

### VertexBufferObject

Hold Vertex buffer object.

	struct VertexBufferObject {

VertexBufferObject(GLsizeiptr size, const GLvoid * data);
		VertexBufferObject(GLsizeiptr size, const GLvoid * data, GLenum usage);
		~VertexBufferObject();
		
		void Bind();
		
		GLuint ID;
	};

where attribute

- __ID__ name the VBO created with VertexBufferObject constructor.

and where methods

- __VertexBufferObject(GLsizeiptr size, const GLvoid * data)__ create a VBO fed with _data_ with a size defined by _size_. By default VBO is setup with _GL_STATIC_DRAW_.
- __VertexBufferObject(GLsizeiptr size, const GLvoid * data, GLenum usage)__ Same as above but setup VBO usage with _usage_, where _usage_ may be one of the following ([see glBufferData man page](https://www.opengl.org/sdk/docs/man4/html/glBufferData.xhtml)):
 - _GL_STREAM_DRAW_
 - _GL_STREAM_READ_
 - _GL_STREAM_COPY_
 - _GL_STATIC_DRAW_
 - _GL_STATIC_READ_
 - _GL_STATIC_COPY_
 - _GL_DYNAMIC_DRAW_
 - _GL_DYNAMIC_READ_
 - _GL_DYNAMIC_COPY_
- __~VertexBufferObject()__ delete VBO named by *ID*.
- __Bind()__ safely make the VBO active. Thrown exception if *ID* doesn't name existing buffer or if is zero with code *DSGL_VBO_DOESNT_EXIST* or *DSGL_VBO_IS_NULL*.

If VBO is not succefully created while instancing class then exception is throw with code error _DSGL_CANNOT_CREATE_VBO_. Attempt to bind VBO that doesn't exist anymore within the object will thrown exception with code error _DSGL_VBO_DOESNT_EXIST_.

### VertexArrayObject

[Vertex Array Object](https://www.opengl.org/wiki/Vertex_Specification#Vertex_Array_Object) store and gather defined states and data passed to your GPU.

DSGL define VAO as shown below

	struct VertexArrayObject {
		VertexArrayObject();
		VertexArrayObject(GLuint IBO, GLuint VBO);
			
		~VertexArrayObject();
			
		void Bind();

		void SetVertex(GLuint vertex);
		void SetElements(GLuint elements);
		void SetInstances(GLuint instances);
		
		static void Unbind();
			
		void AttribPointer(GLuint index,GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer);
		void InstancesAttribPointer(GLuint index,GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer, GLuint divisor);
		
		GLuint ID;
		GLuint IBO;
		GLuint VBO;
		GLuint Instances;

	};
	
where attributes

 - __ID__ name the VAO create by VertexArrayObject constructor.
 - __IBO__ name the IBO given to constructor. It is set to zero if none given.
 - __VBO__ name the VBO given to constructor. It is set to zero if none given.
 
and where methods

 - __VertexArrayObject()__ create a VAO. Thrown exception with code *DSGL_CANNOT_CREATE_VAO* if failed.
 - __VertexArrayObject(GLuint IBO, GLuint VBO)__ create a VAO and bind given IBO and VBO. Like the default constructor it thrown exception with code _DSGL_CANNOT_CREATE_VAO_ if failed. Also it  thrown exception with code _DSGL_IBO_DOESNT_EXIST_ or _DSGL_VBO_DOESNT_EXIST_ if given IBO/VBO doesn't name existing buffer. If one of theses are null exception with code DSGL_IBO_IS_NULL, DSGL_VBO_IS_NULL is raised.
 - __VertexArrayObject(GLuint IBO, GLuint VBO,GLuint instances)__ create a VAO and bind given IBO, instances and VBO. Like the default constructor it thrown exception with code _DSGL_CANNOT_CREATE_VAO_ if failed. Also it  thrown exception with code _DSGL_IBO_DOESNT_EXIST_, _DSGL_IBO_DOESNT_EXIST_ or _DSGL_VBO_DOESNT_EXIST_ if given IBO/instances/VBO doesn't name existing buffer. If one of theses are null exception with code DSGL_IBO_IS_NULL, DSGL_VBO_IS_NULL or DSGL_INSTANCE_IS_NULL is raised.
 - __~VertexArrayObject()__ delete VAO named by *ID*.
 - __AttribPointer(GLuint index,GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer)__ define an array of generic vertex attribute data as defined [there](https://www.opengl.org/sdk/docs/man4/html/glVertexAttribPointer.xhtml) to VAO hold by object. If VAO or VBO doesn't exist it will issue exception with code *DSGL_VAO_DOESNT_EXIST* or _DSGL_VBO_DOESNT_EXIST_. If VBO is null exception with code DSGL_VBO_IS_NULL is raised.
 - __InstancesAttribPointer(GLuint index,GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer, GLuint divisor)__ allow programmer to setup instance buffer attributes. If VAO or instance buffer doesn't exist it will issue exception with code *DSGL_VAO_DOESNT_EXIST* or _DSGL_INSTANCE_DOESNT_EXIST_. If instances is null exception with code DSGL_INSTANCES_IS_NULL is raised.
 - __Bind()__ safely bind VAO. Issue exception if *ID* doesn't name an existing Vertex Array Object with code *DSGL_VAO_DOESNT_EXIST*.
 - __SetInstances(GLuint instances)__ Bind instances buffer to VAO. If VAO or instances buffer doesn't exist it will issue exception with code *DSGL_VAO_DOESNT_EXIST*, *DSGL_INSTANCES_IS_NULL*  or _DSGL_INSTANCES_DOESNT_EXIST_.
 - __SetElementsGLuint elements)__ Bind elements buffer to VAO. If VAO or elements buffer doesn't exist it will issue exception with code *DSGL_VAO_DOESNT_EXIST*, *DSGL_IBO_IS_NULL* or _DSGL_IBO_DOESNT_EXIST_.
 - __SetVertex(GLuint vertex)__ Bind vertex buffer to VAO. If VAO or VBO doesn't exist it will issue exception with code *DSGL_VAO_DOESNT_EXIST*, *DSGL_VBO_IS_NULL* or _DSGL_VBO_DOESNT_EXIST_.
 - __Unbind()__  break currently VAO binding.

### Elements

Hold elements buffer.

	struct Elements {
		Elements(GLsizeiptr size,const GLvoid * data, GLenum usage);
		Elements(GLsizeiptr size,const GLvoid * data);
		
		~Elements();
		
		void Bind();
		static void Unbind();
		
		GLuint ID;
	};

where attributes

 - __ID__ name the elements buffer created by Elements constructor.

and where methods

- __Elements(GLsizeiptr size,const GLvoid * data, GLenum usage)__ create an index buffer and send date to it. *size* specifies the actual size of the buffer ans *usage* specifies the usage pattern of the date store (see [glBufferData](https://www.opengl.org/sdk/docs/man4/html/glBufferData.xhtml)).
- __Elements(GLsizeiptr size,const GLvoid * data) same as above but set usage to GL_STATIC_DRAW by default.
- __~Elements()__ wipe out buffer named by *ID*.
- __Bind()__ make buffer current.

If elements cannot be creates exception is thrown with code *DSGL_CANNOT_CREATE_IBO*. 

### Shader

Hold a single shader of any kind.

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

where attributes

 - __ID__ name the shader create Shader constructor.
 - __Result__ store a boolean telling us if shader compilation successfully complete or not.
 - __shaderSource__ store shader source.
 - __shaderSourceSize__ store shader source size.

and where methods

 - __Shader(const char * inputShader, GLuint shaderType, int option)__ Read input shader create and compile it. Depending of option shader may be read from file or _const char *_ with *DSGL_READ_FROM_FILE* and *DSGL_READ_FROM_STRING*. 
 - __Shader(const char * inputShader, GLuint shaderType)__ Same as above but read shader from _const char *_ by default.
 - __~Shader()__ Delete shader.
 - __ReadFromFile(const char * shaderFilename)__ read shader source and copy its content in member shaderSource.

If shader cannot be created or compiled exception is thrown with code *DSGL_CANNOT_CREATE_SHADER*.

### ShaderProgram

Hold program from linked shader. ShaderProgram is defined as follow

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

where attributes

- __vertex__ store instance of shader initialized with *GL_VERTEX_SHADER*.
- __tesselationControl__ store instance of shader initialized with *GL_TESS_CONTROL_SHADER*.
- __tesselationEvaluation__ store instance of shader initialized with *GL_TELL_EVALUATION_SHADER*.
- __geometry__ store instance of shader initialized with *GL_GEOMETRY_SHADER*.
- __fragment__ store instance of shader initialized with *GL_FRAGMENT_SHADER*.
- __ID__ name the shader program created with constructor.
- __Result__ store a boolean telling if linking successfully complete or not.

and where methods

- __ShaderProgram(const char * inputVertexShader, const char * inputFragmentShader)__ create a shader program with minimal materials given by inputVertexShader and inputFragmentShader. If failed it will issue exception wih code *DSGL_CANNOT_CREATE_PROGRAM* or *DSGL_ERROR_AT_SHDR_COMPILE_TIME*.
- __ShaderProgram(const char * inputVertexShader, const char * inputTesselationControlShader, const char * inputTesselationEvaluationShader, const char * inputGeometryShader, const char * inputFragmentShader)__ same as above but take as inuput every type of shader from the rendering pipeline. Some shaders might be null and will be ignored.
- __~ShaderProgram()__ Drop shader program.
- __Clean(bool shadersOnly)__ Depending of shaderOnly it will drop everything create by object or will erase instanciated shaders. shadersOnly may be *DSGL_CLEAN_ALL* or *DSGL_CLEAN_SHADERS_ONLY*.
- __Use()__ is equivalent to *glUseProgram( ID )* where *ID* is the member of the class.
