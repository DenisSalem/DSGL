#include "dsgl.hpp"

namespace DSGL {
	Context::Context(const char * name, int width, int height, int glMajorVersion, int glMinorVersion) {
		init(name, width, height, glMajorVersion, glMinorVersion);
	}
	
	Context::Context(int width, int height, int glMajorVersion, int glMinorVersion) {
		init("", width, height, glMajorVersion, glMinorVersion);
	}
	
	Context::Init(const char * name, int width, int height, int glMajorVersion, int glMinorVersion) {
		this->name = new char[strlen(name)+1];
		strcpy(this->name, name);	
		this->width = width;
		this->height = height;
		this->glMajorVersion = glMajorVersion;
		this->glMinorVersion = glMinorVersion;
	}
	
	#ifdef DSGL_GLFW
		Context::InitSimpleWindow() {
			if (!glfwInit()) {
				exit(DSGL_GLFW_INIT_FAILED);
			}

			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, this->glMajorVersion);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, this->glMinorVersion);

			this->window = glfwCreateWindow(this->width, this->height, this->name, NULL, NULL);
  
			if (!window)  {
				exit(DSGL_WINDOW_POINTER_NULL);
			}

			glfwMakeContextCurrent(window);

			if(gl3wInit() != 0) {
				exit(DSGL_GL3W_INIT_FAILED);
			}
		}
	#endif
	
	void PrintWorkGroupsCapabilities() {
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
