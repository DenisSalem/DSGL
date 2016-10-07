#include "dsgl.hpp"

namespace DSGL {
	
	/* ----- Context ----- */
	
	Context::Context(const char * name, int width, int height, int glMajorVersion, int glMinorVersion) {
		Init(name, width, height, glMajorVersion, glMinorVersion);
	}
	
	Context::Context(int width, int height, int glMajorVersion, int glMinorVersion) {
		Init("", width, height, glMajorVersion, glMinorVersion);
	}
	
	Context::~Context() {
		#ifdef DSGL_GLFW
			glfwTerminate();
		#endif
		delete this->name;
	}
	
	void Context::Init(const char * name, int width, int height, int glMajorVersion, int glMinorVersion) {
		this->name = new char[strlen(name)+1];
		strcpy(this->name, name);	
		this->width = width;
		this->height = height;
		this->glMajorVersion = glMajorVersion;
		this->glMinorVersion = glMinorVersion;
		this->window = 0;
	}
	
	#ifdef DSGL_GLFW
		int Context::InitSimpleWindow() {
			if (!glfwInit()) {
				throw DSGL_GLFW_INIT_FAILED;
			}

			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, this->glMajorVersion);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, this->glMinorVersion);

			this->window = glfwCreateWindow(this->width, this->height, this->name, NULL, NULL);
  
			if (!window)  {
				throw DSGL_WINDOW_POINTER_NULL;
			}

			glfwMakeContextCurrent(window);

			if(gl3wInit() != 0) {
				throw DSGL_GL3W_INIT_FAILED;
			}
			
			return DSGL_END_NICELY;
		}
	#endif
	
	/* ---- Shader ----- */
	Shader::Shader(const char * inputShader, GLuint shaderType) {
		Init(inputShader, shaderType, DSGL_READ_FROM_FILE);
	}
	
	Shader::Shader(const char * inputShader, GLuint shaderType, int option) {
		Init(inputShader, shaderType, option);
	}
	
	void Shader::ReadFromFile(const char * shaderFilename) {
		FILE * shader = fopen (shaderFilename, "r");
		if (shader == 0) {
			throw DSGL_CANNOT_READ_SHADER_SOURCE;
		}
		this->shaderSourceSize = GetFileSize(shaderFilename)+1;
		this->shaderSource = new char[this->shaderSourceSize];
		for (int i=0; i < this->shaderSourceSize; i++) {
			this->shaderSource[i] = (unsigned char ) fgetc(shader);
			if (this->shaderSource[i] == EOF) {
				this->shaderSource[i] = '\0';
				break;
			}
		}
		
		fclose(shader);
	}
	int Shader::Init(const char * inputShader, GLuint shaderType, int option) {
		if (inputShader == NULL) {
			this->ID = 0;
			return DSGL_END_NICELY;
		}
		
		
		this->Result = GL_FALSE;
		
		/* Create shader */
		this->ID = glCreateShader(shaderType);
		
		if (this->ID == 0) {
			throw DSGL_CANNOT_CREATE_SHADER;
		}	

		if ((option & 1) == 1)  {
			this->shaderSource = new char[strlen(inputShader)+1];
			strcpy(this->shaderSource, inputShader);	
		}

		/* Read from file and load into memory */
		else {
			ReadFromFile(inputShader);
		}

		/* Read from memory and compile */
		glShaderSource(this->ID, 1, &this->shaderSource, NULL);
		glCompileShader(this->ID);
		
		glGetShaderiv(this->ID, GL_COMPILE_STATUS, &this->Result);

		if (!this->Result) {
			this->shaderErrorMessages = new char[DSGL_SHADER_ERROR_LENGTH]() ;
			glGetShaderInfoLog(this->ID, DSGL_SHADER_ERROR_LENGTH, NULL, this->shaderErrorMessages);
			std::cout << inputShader << "\n";
			std::cout <<  this->shaderErrorMessages << "\n" ;
			delete[] this->shaderErrorMessages; this->shaderErrorMessages = NULL;
			delete[] this->shaderSource; this->shaderSource = NULL;
			throw DSGL_ERROR_AT_SHDR_COMPILE_TIME;
		}

		delete[] this->shaderErrorMessages; this->shaderErrorMessages = NULL;
		delete[] this->shaderSource; this->shaderSource = NULL;

		return DSGL_END_NICELY;
	}
	
	Shader::~Shader() {
		delete this->shaderErrorMessages;
		delete this->shaderSource;
		glDeleteShader(this->ID);
	}

	/* ----- ShaderProgram ----- */
	
	ShaderProgram::ShaderProgram() {
	}
	
	ShaderProgram::ShaderProgram(const char * inputVertexShader, const char * inputFragmentShader) {
		Init(inputVertexShader, NULL, NULL, NULL, inputFragmentShader);
	}
	
	ShaderProgram::~ShaderProgram() {
		Clean(DSGL_CLEAN_ALL);
	}
	
	void ShaderProgram::Clean(bool shadersOnly) {
		if (glIsShader(this->vertex->ID)) {
			glDetachShader(this->ID, this->vertex->ID);
		}

		if (glIsShader(this->tesselationControl->ID)) {
			glDetachShader(this->ID, this->tesselationControl->ID);
		}
		
		if (glIsShader(this->tesselationEvaluation->ID)) {
			glDetachShader(this->ID, this->tesselationEvaluation->ID);
		}
		
		if (glIsShader(this->geometry->ID)) {
			glDetachShader(this->ID, this->geometry->ID);
		}
		
		if (glIsShader(this->fragment->ID)) {
			glDetachShader(this->ID, this->fragment->ID);
		}
		
		delete[] this->programErrorMessages;
		delete this->vertex; this->vertex = NULL;
		delete this->tesselationControl; this->tesselationControl = NULL;
		delete this->tesselationEvaluation; this->tesselationEvaluation = NULL;
		delete this->geometry; this->geometry = NULL;
		delete this->fragment; this->fragment = NULL;
		
		if (!shadersOnly) {
			if (glIsProgram(this->ID)) {
				glDeleteProgram(this->ID);
			}			
		}
	}
	
	int ShaderProgram::Init(
		const char * inputVertexShader,
		const char * inputTesselationControlShader,
		const char * inputTesselationEvaluationShader,
		const char * inputGeometryShader,
		const char * inputFragmentShader) {
		
		GLint InfoLogLength = 0;
		
		/* Create program */
		this->ID = glCreateProgram();
		if (!glIsProgram(this->ID)) {
			throw DSGL_CANNOT_CREATE_PROGRAM;
		}	

		/* Create shaders */
		this->vertex = new Shader(inputVertexShader, GL_VERTEX_SHADER, DSGL_READ_FROM_FILE);
		this->tesselationControl = new Shader(inputTesselationControlShader, GL_TESS_CONTROL_SHADER, DSGL_READ_FROM_FILE);
		this->tesselationEvaluation = new Shader(inputTesselationEvaluationShader, GL_TESS_EVALUATION_SHADER, DSGL_READ_FROM_FILE);
		this->geometry = new Shader(inputGeometryShader, GL_GEOMETRY_SHADER, DSGL_READ_FROM_FILE);
		this->fragment = new Shader(inputFragmentShader, GL_FRAGMENT_SHADER, DSGL_READ_FROM_FILE);
		return 0;

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
			Clean(DSGL_CLEAN_ALL);
			throw DSGL_ERROR_AT_SHDR_COMPILE_TIME;
		}

		return DSGL_END_NICELY;
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

		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &workgroup_count[0]);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &workgroup_count[1]);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &workgroup_count[2]);

		printf ("MAXIMUM WORK GROUP COUNT:\n\tx:%u\n\ty:%u\n\tz:%u\n",
		workgroup_size[0], workgroup_size[1], workgroup_size[2]);
		
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &workgroup_size[0]);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &workgroup_size[1]);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &workgroup_size[2]);

		printf ("MAXIMUM WORK GROUP SIZE:\n\tx:%u\n\ty:%u\n\tz:%u\n",
		workgroup_size[0], workgroup_size[1], workgroup_size[2]);

		glGetIntegerv (GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &workgroup_invocations);
		printf ("MAXIMUM WORK GROUP INVOCATIONS:\n\t%u\n", workgroup_invocations);
	}
}
