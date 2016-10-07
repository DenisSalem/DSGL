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
		Init(inputShader, shaderType, DSGL_QUIET);
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
		this->Result = GL_FALSE;
		
		this->verbose = ((option & 1) == 1) ? true : false;
		
		if ((option & 2) == 2)  {
			this->shaderSource = new char[strlen(inputShader)+1];
			strcpy(this->shaderSource, inputShader);	
		}
		/* Read from file and load into memory */
		else {
			ReadFromFile(inputShader);
		}
		
		/* Create shader */		
		this->ID = glCreateShader(shaderType);
		if (this->ID == 0) {
			throw DSGL_CANNOT_CREATE_SHADER;
		}		

		/* Read from memory and compile */
		glShaderSource(this->ID, 1, &this->shaderSource, NULL);
		glCompileShader(this->ID);
		
		if (this->verbose) {
			this->shaderErrorMessages = new char[DSGL_SHADER_ERROR_LENGTH]() ;
			glGetShaderiv(this->ID, GL_COMPILE_STATUS, &this->Result);
			glGetShaderInfoLog(this->ID, DSGL_SHADER_ERROR_LENGTH, NULL, this->shaderErrorMessages);
			if (strlen(this->shaderErrorMessages) != 0 && this->verbose) {
				std::cout << inputShader << "\n";
				std::cout <<  this->shaderErrorMessages << "\n" ;
				delete this->shaderErrorMessages;
				delete this->shaderSource;

				glDeleteShader(this->ID);
				throw DSGL_ERROR_AT_SHDR_COMPILE_TIME;
			}
			delete this->shaderErrorMessages;
			delete this->shaderSource;

		}
		return DSGL_END_NICELY;
	}
	
	Shader::~Shader() {
		glDeleteShader(this->ID);
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
