#include "dsglCubemap.hpp"

DSGL::Cubemap::StamperOnCubeCorner::StamperOnCubeCorner(Gluint localFace) {
	this->localFace = localFace;

	GLint Result = GL_FALSE;
	int InfoLogLength = DSGL_SHADER_ERROR_LENGTH;
	char ProgramErrorMessage[DSGL_SHADER_ERROR_LENGTH] = {0};
  	
	this->stampShaderID = glCreateShader(GL_COMPUTE_SHADER);
  
	if (this->stampShaderID == 0) {
		throw Exception(DSGL_CANNOT_CREATE_SHADER, DSGL_MSG_CANNOT_CREATE_SHADER);
	}
  
	compileShader(this->stampShaderID, this->stampShaderSource);
  
	csProgramID = glCreateProgram();

	glAttachShader(csProgramID, computeShaderID);
	glLinkProgram(csProgramID);
 	glDeleteShader(computeShaderID);
}
