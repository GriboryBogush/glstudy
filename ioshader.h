#ifndef IO_SHADER
#define IO_SHADER

#include"maininc.h"
#include<fstream>
#include<sstream>

int compileShader(const char* filename, GLenum shaderType);

/*
	call cleanShader after this
*/
int linkShader(int vshrhandle, int fshrhandle);
int cleanShader(int shader);


#endif