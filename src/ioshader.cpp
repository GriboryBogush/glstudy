///// A file for abstracting loading shaders a little bit
/////
#include <ioshader.h>
#include <stdexcept>

inline void getProgramLog(int shaderProgram) {
	char infoLog[512];

	glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
	std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
}

inline void getShaderLog(int shader, GLenum shaderType) {
	char infoLog[512];

	std::string shaderName;
	switch (shaderType) {
	case GL_VERTEX_SHADER:
		shaderName = "VERTEX";
		break;
	case GL_FRAGMENT_SHADER:
		shaderName = "FRAGMENT";
		break;
	default:
		throw std::invalid_argument("Invalid shader type");
	}

	glGetShaderInfoLog(shader, 512, NULL, infoLog);
	std::cout << "ERROR::SHADER::" << shaderName << "::COMPILATION_FAILED\n" << infoLog << std::endl;
}

static int getError(int shader, GLenum shaderType=0, bool program=false) {

	int success;
	program ? glGetProgramiv(shader, GL_LINK_STATUS, &success)
			: glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success) {
		program ? getProgramLog(shader) : getShaderLog(shader, shaderType);
		return -1;
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

