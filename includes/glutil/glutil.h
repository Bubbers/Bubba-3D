#ifndef __LAB_GL_UTILS_H__
#define __LAB_GL_UTILS_H__

/** This file contains utility functions to be used for the labs in computer
 * graphics at chalmers, they are not covered by any particular license...
 */

#include "float4x4.h"
#include "float3.h"

#include <string>
#include <cassert>

#include <GL/glew.h>



// Sometimes it exists, sometimes not...
#ifndef M_PI
#	define M_PI 3.14159265358979323846f
#endif 

/* Create a matrix used for perspective projection.
 *
 *  - fov : field of view, in degrees
 *  - aspectRatio : aspect ratio, ratio of width to height
 *	- n : distance to near plane
 *	- f : distance to far plane
 */
chag::float4x4 perspectiveMatrix(float fov, float aspectRatio, float n, float f);

/* LookAt Matrix
 *
 * TBD
 */
chag::float4x4 lookAt(const chag::float3 &eyePosition, const chag::float3 &lookAt, const chag::float3 &desiredUp);

/** This macro checks for GL errors using glGetError().
 *
 * If you're unsure where to put this, place it after every call to GL. If a
 * debugger is attached, CHECK_GL_ERROR() will break on the offending line, and
 * print the file and line location in an MSVC compatible format on the debug
 * output and console.
 *
 * Note: CHECK_GL_ERROR() will report any errors since the last call to
 * glGetError()! If CHECK_GL_ERROR() reports an error, you *must* consider all
 * calls to GL since the last CHECK_GL_ERROR() (or call to glGetError())!
 *
 * Note: the macro _cannot_ be used between glBegin() and glEnd(), as stated in
 * the OpenGL standard.
 * 
 * Example (we're looking for an error at either glClearColor() or glClear()): 
 *	CHECK_GL_ERROR(); // catch previous errors
 *	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
 *	CHECK_GL_ERROR(); // see if glClearColor() generated an error
 *	glClear(GL_COLOR_BUFFER_BIT);
 *	CHECK_GL_ERROR(); // see if glClear() generated an error
 */

#define CHECK_GL_ERROR() { checkGLError(__FILE__, __LINE__) && (__debugbreak(), 1); }

#if !defined(_WIN32)
#	define __debugbreak() assert(false)
#endif

/**
 * Internal function used by macro CHECK_GL_ERROR, use that instead.
 */
bool checkGLError(const char *file, int line);

/**
 * Print GL version/vendor and renderer. Ensure that we've got OpenGL 3.0
 * (GLEW_VERSION_3_0). Bail if we don't.
 */
void startupGLDiagnostics();

void checkMinMax(float x, float y, float z, chag::float3* minV, chag::float3* maxV);

/** 
 * Initialize OpenGL debug messages.
 */
void setupGLDebugMessages();

/** 
 * Error reporting function
 */
void fatal_error( std::string errorString, std::string title = std::string() );

/**
 * Helper function: creates a cube map using the files specified for each face.
 */
GLuint loadCubeMap(const char* facePosX, const char* faceNegX, const char* facePosY, const char* faceNegY, const char* facePosZ, const char* faceNegZ);


/**
 * Helper function used to create a shader object from text in a file
 */
const char *textFileRead( const char *fn, bool fatalError = true );


/**
 * Helper function used to get log info (such as errors) about a shader object or shader program
 */
std::string GetShaderInfoLog(GLuint obj);

/**
 * Loads and compiles a fragment and vertex shader. Then creates a shader program
 * and attaches the shaders. Does NOT link the program, this is done with  linkShaderProgram()
 * The reason for this is that before linking we need to bind attribute locations, using
 * glBindAttribLocation and fragment data lications, using glBindFragDataLocation.
 */
GLuint loadShaderProgram(const std::string &vertexShader, const std::string &fragmentShader);
/**
 * Call to link a shader program prevoiusly loaded using loadShaderProgram.
 */
void linkShaderProgram(GLuint shaderProgram);

/**
 * Creates a GL buffer and uploads the given data to it.
 * returns the handle of the GL buffer.
 */
GLuint createAddAttribBuffer(GLuint vertexArrayObject, const void *data, const size_t dataSize, GLuint attributeIndex, GLsizei attributeSize, GLenum type, GLenum bufferUsage = GL_STATIC_DRAW);


/**
 * Helper to set uniform variables in shaders, labeled SLOW because they find the location from string each time.
 * In OpenGL (and similarly in other APIs) it is much more efficient (in terms of CPU time) to keep the uniform
 * location, and use that. Or even better, use uniform buffers!
 * However, in the simple tutorial samples, performance is not an issue.
 * Overloaded to set many types.
 */
void setUniformSlow(GLuint shaderProgram, const char *name, const chag::float4x4 &matrix);
void setUniformSlow(GLuint shaderProgram, const char *name, const float value);
void setUniformSlow(GLuint shaderProgram, const char *name, const GLint value);
void setUniformSlow(GLuint shaderProgram, const char *name, const chag::float3 &value);

/**
 * Code that draws a sphere where the light is and a stippled line to the 
 * origo. Note that this method uses functions that have been deprecated in
 * OpenGL 3 and so would not necessarily work on all future implementations. 
 * 
 * That said, for drawing simple debug information such as this, OpenGL 2
 * can be pretty handy.
 */
void debugDrawLight(const chag::float4x4 &viewMatrix, 
					const chag::float4x4 &projectionMatrix, 
					const chag::float3 &worldSpaceLightPos);




#if defined(__linux__)
/* Change CWD (current working directory) under linux. 
 *
 * Data files are either in the same directory as the binary (here referred to
 * as ${BINDIR}, or if installed using `make install' or similar, they should
 * be at ${BINDIR}/../share/<lab name>/. 
 *
 * linux_initialize_cwd() checks if ${BINDIR}/../share/<lab name> exists, and
 * if so, changes the CWD to that. Otherwise it changes to ${BINDIR}.
 */
void linux_initialize_cwd();
#endif // ! __linux__

#endif // LAB_GL_UTILS_H
