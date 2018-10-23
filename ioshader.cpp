///// A file for abstracting loading shaders a little bit
/////
#include"ioshader.h"

static int getError(int shader, GLenum shaderType, bool program=false) {

	int success;
	char infoLog[512];
	
	if (program) {

		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
			return -1;
		}

	}
	else {

		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {

			char* str;
			char vstr[] = "VERTEX";
			char fstr[] = "FRAGMENT";

			if (shaderType == GL_VERTEX_SHADER) {
				str = vstr;
			}
			else if (shaderType = GL_FRAGMENT_SHADER) {
				str = fstr;
			}

			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::" << str << "::COMPILATION_FAILED\n" << infoLog << std::endl;
			return -1;
		}


	}

	return success;

}

int compileShader(const char* filename, GLenum shaderType) {
	
	int shader = glCreateShader(shaderType);

	std::ifstream shaderFile;

	shaderFile.open(filename);
	std::stringstream shrstream;

	shrstream << shaderFile.rdbuf();
	shaderFile.close();

	std::string src = shrstream.str();
	const char* shrsource = src.c_str();
	
	glShaderSource(shader, 1, &shrsource, nullptr);
	glCompileShader(shader);

	if (getError(shader, shaderType) == -1) {
		return - 1;
	}

	return shader;

}

int linkShader(int vshrhandle, int fshrhandle) {

	int shrprogram = glCreateProgram();

	glAttachShader(shrprogram, vshrhandle);
	glAttachShader(shrprogram, fshrhandle);
	glLinkProgram(shrprogram);

	if (getError(shrprogram, NULL, true) == -1) {
		return - 1;
	}

	return shrprogram;
}

int cleanShader(int shader) {
	glDeleteShader(shader);
	return 0;
}

