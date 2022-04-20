///// A file to load shaders, it'll print out error if anything goes
///// wrong. For now that's all the error handling I need
#ifndef IO_SHADER
#define IO_SHADER

#include"maininc.h"
#include<fstream>
#include<sstream>

int compileShader(const char* filename, GLenum shaderType);

// Call cleanShader after this
int linkShader(int vshrhandle, int fshrhandle);
int cleanShader(int shader);


#endif