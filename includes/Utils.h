#ifndef __UTILS_H__
#define __UTILS_H__

#include <GL/freeglut.h>

//*****************************************************************************
//	Frame buffer objects
//*****************************************************************************
struct Fbo{
	GLuint id;
	GLuint texture;
	GLuint shaderProgram;
	GLuint depthbuffer;

	int width, height;
};

#endif