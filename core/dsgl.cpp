#include "dsgl.hpp"

namespace DSGL {
	
	/* ----- Exception ----- */
	
	Exception::Exception(int code, const char* msg) : Exception(code, msg, NULL) {}
	
	Exception::Exception(int code, const char * msg, const char * filename) {
		this->code = code;
		this->msg = std::string(msg);
		#ifdef DSGL_DEBUG
			if (filename != NULL) {
				this->filename = std::string(filename);
				std::cerr << "DSGL: " << this->filename << "\n";
			}
			std::cerr << this->msg << "\n";
		#endif
	}
	
	/* ----- Context ----- */
	
	Context::Context(const char * name, int width, int height, int glMajorVersion, int glMinorVersion) {
		this->name = std::string(name);
		this->width = width;
		this->height = height;
		this->glMajorVersion = glMajorVersion;
		this->glMinorVersion = glMinorVersion;
		this->window = 0;
	}
	
	Context::Context(int width, int height, int glMajorVersion, int glMinorVersion) : Context("", width, height, glMajorVersion, glMinorVersion) {}
	
	Context::~Context() {
		#ifdef DSGL_GLFW
			glfwTerminate();
		#endif
	}
	
	void Context::InitSimpleWindow() {
		#if defined(DSGL_GLFW)
			if (!glfwInit()) {
				throw Exception(DSGL_GLFW_INIT_FAILED, DSGL_MSG_GLFW_INIT_FAILED);
			}

			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, this->glMajorVersion);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, this->glMinorVersion);

			this->window = glfwCreateWindow(this->width, this->height, this->name.c_str(), NULL, NULL);

			if (!window)  {
				throw Exception(DSGL_WINDOW_POINTER_NULL, DSGL_MSG_WINDOW_POINTER_NULL);
			}

			glfwMakeContextCurrent(window);
		#endif
		#if defined(DSGL_GL3W)
		  if(gl3wInit() != 0) {
		    throw Exception(DSGL_GL3W_INIT_FAILED,"DSGL: GL3W initialization failed.");
		  }
		#endif
	}
	
	/* ----- VertexBufferObject ----- */
	
        VertexBufferObject::VertexBufferObject(GLsizeiptr size, const GLvoid * data) : VertexBufferObject(size, data, GL_STATIC_DRAW) {}
	
	VertexBufferObject::VertexBufferObject(GLsizeiptr size, const GLvoid * data, GLenum usage) {
		glGenBuffers(1, &this->ID);
		glBindBuffer(GL_ARRAY_BUFFER, this->ID);
			if(glIsBuffer(this->ID)) {
				glBufferData(GL_ARRAY_BUFFER, size, data, usage);
			}
			else {
				throw Exception(DSGL_CANNOT_CREATE_VBO,"DSGL: VBO creation failed.");
			}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	
	VertexBufferObject::~VertexBufferObject() {
		glDeleteBuffers(1, &this->ID);
	}
	
	void VertexBufferObject::Bind() {
		if(this->ID != 0) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ID);
			if(!glIsBuffer(this->ID)) {
				throw Exception(DSGL_VBO_DOESNT_EXIST, "DSGL: VBO doesn't exist.");
			}
		}
		else {
			throw Exception(DSGL_VBO_IS_NULL, DSGL_MSG_VBO_IS_NULL);
		}
	}
	
	void VertexBufferObject::Unbind() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);		
	}
	
	/* ---- VertexArrayObject ----- */
	
	VertexArrayObject::VertexArrayObject() {
		glGenVertexArrays(1, &this->ID);
		this->Bind();
		this->Unbind();	
	}
	
	VertexArrayObject::VertexArrayObject(GLuint IBO, GLuint VBO) {
		glGenVertexArrays(1, &this->ID);
		this->Bind();			
			this->SetVertex(VBO);
			this->SetElements(IBO);
		this->Unbind();	
	}
	
	VertexArrayObject::VertexArrayObject(GLuint IBO, GLuint VBO, GLuint instances) {
		glGenVertexArrays(1, &this->ID);
		this->Bind();			
			this->SetVertex(VBO);
			this->SetElements(IBO);
			this->SetInstances(instances);
		this->Unbind();	
		
	}
	
	void VertexArrayObject::Bind() {
		glBindVertexArray(this->ID);
		if(!glIsVertexArray(this->ID)) {
			throw Exception(DSGL_VAO_DOESNT_EXIST, "DSGL: VAO doesn't exist.");			
		}
	}
	
	void VertexArrayObject::SetVertex(GLuint vertex) {
		this->Bind();
			if (vertex == 0) {
				throw Exception(DSGL_VBO_IS_NULL, DSGL_MSG_VBO_IS_NULL);
			}
			glBindBuffer(GL_ARRAY_BUFFER, vertex);
			if(!glIsBuffer(vertex)) {
				throw Exception(DSGL_VBO_DOESNT_EXIST, "DSGL: Vertex buffer doesn't exist.");
			}
			this->VBO = vertex;
		this->Unbind();	
	}
	void VertexArrayObject::SetElements(GLuint elements) {
		this->Bind();
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elements);
			if(elements != 0 && !glIsBuffer(elements)) {
				throw Exception(DSGL_IBO_DOESNT_EXIST, "DSGL: Elements buffer doesn't exist.");
			}
			this->IBO = elements;
		this->Unbind();
	}
	
	void VertexArrayObject::SetInstances(GLuint instances) {
		this->Bind();
			if (instances == 0) {
				throw Exception(DSGL_INSTANCES_IS_NULL, "DSGL: instance buffer name is null.");
			}
			glBindBuffer(GL_ARRAY_BUFFER, instances);
			if(!glIsBuffer(instances)) {
				throw Exception(DSGL_INSTANCES_DOESNT_EXIST, "DSGL: instances buffer doesn't exist.");
			}
			this->instances = instances;
		this->Unbind();
	}
	
	void VertexArrayObject::Unbind() {
		glBindVertexArray(0);
	}
	
	void VertexArrayObject::AttribPointer(GLuint index,GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer) {
		if(!glIsVertexArray(this->ID)) {
			throw Exception(DSGL_VAO_DOESNT_EXIST, "DSGL: VAO doesn't exist.");			
		}
		glBindVertexArray(this->ID);
			glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
			if(!glIsBuffer(this->VBO)) {
				throw Exception(DSGL_VBO_DOESNT_EXIST, "DSGL: Vertex buffer doesn't exist.");
			}
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, size, type, normalized, stride, pointer);
		glBindVertexArray(0);
	}
	
	void VertexArrayObject::InstancesAttribPointer(GLuint index,GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer, GLuint divisor) {
		if(!glIsVertexArray(this->ID)) {
			throw Exception(DSGL_VAO_DOESNT_EXIST, "DSGL: VAO doesn't exist.");			
		}
		glBindVertexArray(this->ID);
			glBindBuffer(GL_ARRAY_BUFFER, this->instances);
			if(!glIsBuffer(this->instances)) {
				throw Exception(DSGL_INSTANCES_DOESNT_EXIST, "DSGL: instances buffer doesn't exist.");
			}
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, size, type, normalized, stride, pointer);
			glVertexAttribDivisor(index, divisor);
		glBindVertexArray(0);
	}
	
	VertexArrayObject::~VertexArrayObject() {
		glDeleteVertexArrays(1,&this->ID);
	}
	
	/* ---- Elements ----- */
	
	Elements::Elements(GLsizeiptr size, const GLvoid * data) : Elements(size, data, GL_STATIC_DRAW){}
	
	Elements::Elements(GLsizeiptr size, const GLvoid * data, GLenum usage) {
		glGenBuffers(1, &this->ID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ID);
			if(glIsBuffer(this->ID)) {
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);
			}
			else {
				throw Exception(DSGL_CANNOT_CREATE_IBO, "DSGL: Elements buffer creation failed.");
			}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	
	Elements::~Elements() {
		glDeleteBuffers(1, &this->ID);
	}
	
	void Elements::Bind() {
		if (this->ID != 0) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ID);
			if(!glIsBuffer(this->ID)) {
				throw Exception(DSGL_IBO_DOESNT_EXIST, "DSGL: Elements buffer creation failed.");
			}
		}
		else {
			throw Exception(DSGL_IBO_IS_NULL, "DSGL: IBO is null.");
		}
	}
	
	void Elements::Unbind() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);		
	}
	
	/* ---- Shader ----- */
		
	Shader::Shader(const char * inputShader, GLuint shaderType, int option) {
		if (inputShader == NULL) {
			this->ID = 0;
			return;
		}
				
		this->Result = GL_FALSE;
		
		/* Create shader */
		this->ID = glCreateShader(shaderType);
		
		if (this->ID == 0) {
			if ((option & 1) == 0) {
				throw Exception(DSGL_CANNOT_CREATE_SHADER, "DSGL: Cannot create shader.", inputShader);
			}
			else {
				throw Exception(DSGL_CANNOT_CREATE_SHADER, "DSGL: Cannot create shader.");
			}
		}	

		/* Copy from existing string */
		if ((option & 1) == 1)  {
			this->shaderSource = std::string(inputShader);
		}

		/* Read from file and load into memory */
		else {
			ReadFromFile(inputShader);
		}

		/* Read from memory and compile */
		const char * shaderSource_ptr = this->shaderSource.c_str();
		glShaderSource(this->ID, 1, &shaderSource_ptr, NULL);
		glCompileShader(this->ID);
		
		glGetShaderiv(this->ID, GL_COMPILE_STATUS, &this->Result);

		if (!this->Result) {
			this->shaderErrorMessages = new char[DSGL_SHADER_ERROR_LENGTH]() ;
			glGetShaderInfoLog(this->ID, DSGL_SHADER_ERROR_LENGTH, NULL, this->shaderErrorMessages);
			std::cout <<  this->shaderErrorMessages << "\n" ;
			delete[] this->shaderErrorMessages; this->shaderErrorMessages = NULL;
			if ((option & 1) == 0) {
				throw Exception(DSGL_ERROR_AT_SHDR_COMPILE_TIME, "DSGL: Cannot compile shader.", inputShader);
			}
			else {
				throw Exception(DSGL_ERROR_AT_SHDR_COMPILE_TIME, "DSGL: Cannot compile shader.");
			}
		}
	}
	
	Shader::Shader(const char * inputShader, GLuint shaderType) : Shader(inputShader, shaderType, DSGL_READ_FROM_FILE) {}
	
	void Shader::ReadFromFile(const char * shaderFilename) {
		FILE * shader = fopen (shaderFilename, "r");
		char * stringBuffer = NULL;
		if (shader == 0) {
			throw Exception(DSGL_CANNOT_READ_SHADER_SOURCE, "DSGL: Cannot read shader source.", shaderFilename);
		}
		this->shaderSourceSize = GetFileSize(shaderFilename)+1;
		stringBuffer = new char[this->shaderSourceSize];
		for (int i=0; i < this->shaderSourceSize; i++) {
			stringBuffer[i] = (unsigned char ) fgetc(shader);
			if (stringBuffer[i] == EOF) {
				stringBuffer[i] = '\0';
				break;
			}
		}
		
		this->shaderSource.assign(stringBuffer, this->shaderSourceSize);
		
		fclose(shader);
		delete[] stringBuffer;
	}
	
	Shader::~Shader() {
		glDeleteShader(this->ID);
	}

	/* ----- ShaderProgram ----- */
	
	ShaderProgram::ShaderProgram(const char * inputVertexShader, const char * inputFragmentShader) : ShaderProgram(inputVertexShader, NULL, NULL, NULL, inputFragmentShader) {}
	
	ShaderProgram::ShaderProgram(
		const char * inputVertexShader,
		const char * inputTesselationControlShader,
		const char * inputTesselationEvaluationShader,
		const char * inputGeometryShader,
		const char * inputFragmentShader) {
		
		GLint InfoLogLength = 0;
		
		/* Create program */
		this->ID = glCreateProgram();
		if (!glIsProgram(this->ID)) {
			throw Exception(DSGL_CANNOT_CREATE_PROGRAM, "DSGL: Cannot create program");
		}	

		/* Create shaders */
		this->vertex = std::make_shared<Shader>(inputVertexShader, GL_VERTEX_SHADER, DSGL_READ_FROM_FILE);
		this->tesselationControl = std::make_shared<Shader>(inputTesselationControlShader, GL_TESS_CONTROL_SHADER, DSGL_READ_FROM_FILE);
		this->tesselationEvaluation = std::make_shared<Shader>(inputTesselationEvaluationShader, GL_TESS_EVALUATION_SHADER, DSGL_READ_FROM_FILE);
		this->geometry = std::make_shared<Shader>(inputGeometryShader, GL_GEOMETRY_SHADER, DSGL_READ_FROM_FILE);
		this->fragment = std::make_shared<Shader>(inputFragmentShader, GL_FRAGMENT_SHADER, DSGL_READ_FROM_FILE);

		/* Link and compile */

		if (glIsShader(this->vertex->ID)) {
			glAttachShader(this->ID, this->vertex->ID);
		}

		if (glIsShader(this->tesselationControl->ID)) {
			glAttachShader(this->ID, this->tesselationControl->ID);
		}
		
		if (glIsShader(this->tesselationEvaluation->ID)) {
			glAttachShader(this->ID, this->tesselationEvaluation->ID);
		}
		
		if (glIsShader(this->geometry->ID)) {
			glAttachShader(this->ID, this->geometry->ID);
		}
		
		if (glIsShader(this->fragment->ID)) {
			glAttachShader(this->ID, this->fragment->ID);
		}
		
		glLinkProgram(this->ID);
		
		/* Clean shaders */
		Clean(DSGL_CLEAN_SHADER_ONLY);

		glGetProgramiv(this->ID, GL_LINK_STATUS, &this->Result);

		if (!Result) {
			glGetProgramiv(this->ID, GL_INFO_LOG_LENGTH, &InfoLogLength);
			this->programErrorMessages = new char[DSGL_SHADER_ERROR_LENGTH]() ;
			glGetProgramInfoLog(this->ID, GL_INFO_LOG_LENGTH, NULL, &this->programErrorMessages[0]);
			std::cout << this->programErrorMessages << "\n";
			delete[] this->programErrorMessages;
			Clean(DSGL_CLEAN_ALL);
			throw DSGL_ERROR_AT_SHDR_COMPILE_TIME;
		}
	}	
	
	ShaderProgram::~ShaderProgram() {
		Clean(DSGL_CLEAN_ALL);
	}
	
	void ShaderProgram::Use() {
		if(glIsProgram(this->ID)) {
			glUseProgram(this->ID);
		}
		else {
			throw Exception(DSGL_ID_DOESNT_NAME_A_PROGRAM, "DSGL: ID doesn't name a program.");
		}
	}
	
	void ShaderProgram::Clean(bool shadersOnly) {
		if (glIsShader(this->vertex->ID)) {
			glDetachShader(this->ID, this->vertex->ID);
			glDeleteShader(this->vertex->ID);
		}

		if (glIsShader(this->tesselationControl->ID)) {
			glDetachShader(this->ID, this->tesselationControl->ID);
			glDeleteShader(this->tesselationControl->ID);
		}
		
		if (glIsShader(this->tesselationEvaluation->ID)) {
			glDetachShader(this->ID, this->tesselationEvaluation->ID);
			glDeleteShader(this->tesselationEvaluation->ID);
		}
		
		if (glIsShader(this->geometry->ID)) {
			glDetachShader(this->ID, this->geometry->ID);
			glDeleteShader(this->geometry->ID);
		}
		
		if (glIsShader(this->fragment->ID)) {
			glDetachShader(this->ID, this->fragment->ID);
			glDeleteShader(this->fragment->ID);
		}
		
		if (!shadersOnly) {
			if (glIsProgram(this->ID)) {
				glDeleteProgram(this->ID);
			}			
		}
	}
	
	/* ----- Miscellaneous functions ----- */
	
	int GetFileSize(const char * inputFilePath) {
		// http://www.cplusplus.com/doc/tutorial/files/
		std::streampos begin, end;
		std::ifstream inputFile(inputFilePath, std::ios::binary);
		begin = inputFile.tellg();
		inputFile.seekg(0, std::ios::end);
		end = inputFile.tellg();
		inputFile.close();
		return int(end - begin);
	}

	void PrintNicelyWorkGroupsCapabilities() {
		int workgroup_count[3];
		int workgroup_size[3];
		int workgroup_invocations;

		for (int i=0; i < 3; i++) {	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, i, &workgroup_count[i]); }

		printf ("DSGL: MAXIMUM WORK GROUP COUNT:\n\tx:%u\n\ty:%u\n\tz:%u\n",
		workgroup_size[0], workgroup_size[1], workgroup_size[2]);
		
		for (int i=0; i < 3; i++) { glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, i, &workgroup_size[i]); }

		printf ("DSGL: MAXIMUM WORK GROUP SIZE:\n\tx:%u\n\ty:%u\n\tz:%u\n",
		workgroup_size[0], workgroup_size[1], workgroup_size[2]);

		glGetIntegerv (GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &workgroup_invocations);
		printf ("DSGL: MAXIMUM WORK GROUP INVOCATIONS:\n\t%u\n", workgroup_invocations);
	}
}
