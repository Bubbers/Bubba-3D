

#ifdef WIN32
  #define GLEW_STATIC
  #define WIN32_LEAN_AND_MEAN
  #define VC_EXTRALEAN
  #define NOMINMAX //          - Macros min(a,b) and max(a,b)
#include <windows.h>
#endif // WIN32

#ifdef __linux__
#	include <unistd.h>
#endif // ! __linux__

#include <GL/glew.h>
#include <GL/glut.h>


#include "float2.h"
#include "float3x3.h"
#include "glutil.h"

#include <cmath>
#include <cstring>
#include <cstdlib>
#include <stdio.h>

#include <vector>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <Logger.h>

using std::vector; 
using namespace chag; 


float4x4 perspectiveMatrix(float fov, float aspectRatio, float n, float f) /* field of view, aspect ratio, near clipping plane, far clipping plane */
{
	// This matrix is created identically to gluPerspective()
	// and takes identical parameters.
	return make_perspective(fov, aspectRatio, n, f);
}

void checkMinMax(float x, float y, float z, chag::float3* minV, chag::float3* maxV) {
	if (x < minV->x) { minV->x = x; };
	if (y < minV->y) { minV->y = y; };
	if (z < minV->z) { minV->z = z; };
	if (x > maxV->x) { maxV->x = x; };
	if (y > maxV->y) { maxV->y = y; };
	if (z > maxV->z) { maxV->z = z; };
}



float4x4 lookAt(const float3 &eye, const float3 &center, const float3 &up)
{
	float3 dir = chag::normalize(eye - center);
	float3 right = chag::normalize(cross(up, chag::normalize(dir)));
	float3 newup = chag::normalize(cross(dir, right));
	float3x3 R = make_matrix(right, newup, dir);
	float4x4 invrot = make_matrix(transpose(R), make_vector(0.0f,0.0f,0.0f));
	return invrot * make_translation(-eye); 
}


bool checkGLError(const char *file, int line)
{
  bool wasError = false;

  for (GLenum glErr = glGetError(); glErr != GL_NO_ERROR; glErr = glGetError())
  {
    wasError = true;
    const GLubyte* sError = gluErrorString(glErr);
    
    if (!sError)
    {
      sError = reinterpret_cast<const GLubyte *>(" (no message available)");
    }

    std::cerr << "GL Error #" << glErr << "(" << sError << ") " << " in File " << file << " at line: " << line << std::endl;

#if defined(_WIN32)
    std::stringstream ss;
    ss  << file << "(" << line << "): GL Error #" << glErr << "(" << sError << ") " << std::endl;

    // outputs error message to debug console, if a debugger is attached.
    OutputDebugStringA(ss.str().c_str());
#endif
  }
  return wasError;
}

// startupGLDiagnostics
void startupGLDiagnostics()
{
	// print diagnostic information
	printf( "GL  VENDOR: %s\n", glGetString( GL_VENDOR ) );
	printf( "   VERSION: %s\n", glGetString( GL_VERSION ) );
	printf( "  RENDERER: %s\n", glGetString( GL_RENDERER ) );

	// test if we've got GL 3.0
	if( !GLEW_VERSION_3_0 )
	{
		fatal_error( "OpenGL 3.0 not supported.\n" 
			"Please update your drivers and/or buy a better graphics card."
		);
	}
}

// setupGLDebugMessages
namespace 
{
#	if defined(_WIN32)
#		define CALLBACK_ CALLBACK
#	else // !win32
#		define CALLBACK_
#	endif // ~ platform

	/* Callback function. This function is called by GL whenever it generates
	 * a debug message.
	 */
	GLvoid CALLBACK_ handle_debug_message_( GLenum aSource, GLenum aType, GLuint aId, GLenum aSeverity, GLsizei /*aLength*/, GLchar const* aMessage, GLvoid* /*aUser*/ )
	{
		// source string
		const char* srcStr = NULL;
		switch (aSource) {
		case GL_DEBUG_SOURCE_API:				srcStr = "API";	break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:		srcStr = "WINDOW_SYSTEM"; break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER:	srcStr = "SHADER_COMPILER"; break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:		srcStr = "THIRD_PARTY"; break;
		case GL_DEBUG_SOURCE_APPLICATION:		srcStr = "APPLICATION"; break;
		case GL_DEBUG_SOURCE_OTHER:				srcStr = "OTHER"; break;
		default:								srcStr = "UNKNOWN";	break;
		}

		// type
		const char* typeStr = NULL;
		switch (aType)	{
		case GL_DEBUG_TYPE_ERROR:				typeStr = "ERROR";	break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:	typeStr = "DEPRECATED_BEHAVIOR";	break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:	typeStr = "UNDEFINED_BEHAVIOR";	break;
		case GL_DEBUG_TYPE_PORTABILITY:			typeStr = "PORTABILITY";	break;
		case GL_DEBUG_TYPE_PERFORMANCE:			typeStr = "PERFORMANCE";	break;
		case GL_DEBUG_TYPE_OTHER:				typeStr = "OTHER"; break;
		default:								typeStr = "UNKNOWN";	break;
		}

		// severity
		const char* sevStr = NULL;
		switch( aSeverity ) {
		case GL_DEBUG_SEVERITY_HIGH:			sevStr = "HIGH"; break;
		case GL_DEBUG_SEVERITY_MEDIUM:			sevStr = "MEDIUM"; break;
		case GL_DEBUG_SEVERITY_LOW:				sevStr = "LOW";	break;
		case GL_DEBUG_SEVERITY_NOTIFICATION:	sevStr = "NOTIFICATION"; break;
		default:								sevStr = "UNKNOWN";
		}

		// output message, if not just notification
		if (aSeverity != GL_DEBUG_SEVERITY_NOTIFICATION) {
			std::stringstream szs;
			szs << "\n"
				<< "--\n"
				<< "-- GL DEBUG MESSAGE:\n"
				<< "--   severity = '" << sevStr << "'\n"
				<< "--   type     = '" << typeStr << "'\n"
				<< "--   source   = '" << srcStr << "'\n"
				<< "--   id       = " << std::hex << aId << "\n"
				<< "-- message:\n"
				<< aMessage << "\n"
				<< "--\n"
				<< "\n"
				;

			//fprintf(stderr, "%s", szs.str().c_str());
			Logger::logDebug(szs.str().c_str());
			fflush(stderr);
		}

		// Additionally: if it's (really) bad -> break!
		if(aSeverity == GL_DEBUG_SEVERITY_HIGH)	{
#			if defined(_WIN32)
			__debugbreak();
#			else // !win32
			assert(false);
#			endif // ~ platform
		}
	}

	// cleanup macros
#	undef CALLBACK_
}

void setupGLDebugMessages()
{
	// Check for GL errors prior to us.
	CHECK_GL_ERROR();

	/* Make sure that we support this extension before attempting to do any-
	 * thing with it...
	 */
	if( !glewIsExtensionSupported( "GL_ARB_debug_output" ) )
	{
		printf( " --- \n" );
		printf( " --- \n" );
		printf( " --- BIG WARNING: GL_ARB_debug_output not supported!\n" );
		printf( " --- This is rather bad news.\n" ); /* TODO: give link to doc explaining this */
		printf( " --- \n" );
		printf( " --- \n" );
		return;
	}

	/* Enable debug messages. Set a callback handle, and then enable all 
	 * messages to begin with.
	 */
	glDebugMessageCallbackARB((GLDEBUGPROC)handle_debug_message_, 0 );
	//glDebugMessageCallback((GLDEBUGPROC)handle_debug_message_, 0);
	glDebugMessageControlARB( GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, 0, true );
	
	/* Enable synchronous debug output; this causes the callback to be called
	 * immediately on error, usually in the actual gl-function where the error
	 * occurred. (So, your stack trace is actually useful).
	 *
	 * This comes at a performance cost, so for performance sensitive stuff,
	 * it might not be a good idea to unconditionally enable this. For the
	 * labs, we just enable it, however.
	 */
	glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB );

	/* Debug output can be somewhat spammy, especially if all messages are 
	 * enabled. For now, disable the lowest level of messages, which mostly
	 * contain performance-related information and other random notes.
	 */
	glDebugMessageControlARB( GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_LOW_ARB, 0, 0, false );

	/* Now, check if enabling debug messages caused a GL error. If so, that
	 * error might not be reported by the debug message mechanism (after all,
	 * the error occurred while setting it up). Later errors should be reported
	 * via the callbacks, though.
	 */
	CHECK_GL_ERROR();
}

// Error reporting
void fatal_error( std::string errorString, std::string title )
{
	if (title.empty())
	{
		title = "GL-Tutorial - Error";
	}
	if (errorString.empty())
	{
		errorString = "(unknown error)";
	}

	// On Win32 we'll use a message box. On !Win32, just print to stderr and abort()
#if defined(_WIN32)
	MessageBox( 0, errorString.c_str(), title.c_str(), MB_OK | MB_ICONEXCLAMATION );
#else
	fprintf( stderr, "%s : %s\n", title.c_str(), errorString.c_str() );
#endif

	abort();
}


const char *textFileRead( const char *fn, bool fatalError )
{
	/* Note: the `fatalError' thing is a bit of a hack, The proper course of
	 * action would be to have people check the return values of textFileRead,
	 * but lets avoid cluttering the lab-code even further.
	 */

	FILE *fp;
	char *content = NULL;
	int count = 0;

	if( fn != NULL ) 
	{
		fp = fopen( fn, "rt" );
		if( fp != NULL ) 
		{
			fseek( fp, 0, SEEK_END );
			count = ftell( fp );
			fseek( fp, 0, SEEK_SET );

			if( count > 0 ) 
			{
				content = new char[count+1];
				count = fread( content, sizeof(char), count, fp );
				content[count] = '\0';
			}
			else
			{
				if( fatalError )
				{
					char buffer[256];
					sprintf( buffer, "File '%s' is empty\n", fn );
					fatal_error( buffer );
				}
			}

			fclose( fp );
		}
		else
		{
			if( fatalError )
			{
				char buffer[256];
				sprintf( buffer, "Unable to read file '%s'\n", fn );
				fatal_error( buffer );
			}
		}
	}
	else
	{
		if( fatalError )
			fatal_error( "textFileRead - argument NULL\n" );
	}

	return content;
}



std::string GetShaderInfoLog(GLuint obj) {
	int logLength = 0;
	int charsWritten  = 0;
	char *tmpLog;
	std::string log;

	glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &logLength);

	if (logLength > 0) {
		tmpLog = new char[logLength];
		glGetShaderInfoLog(obj, logLength, &charsWritten, tmpLog);
		log = tmpLog;
		delete[] tmpLog;
	}

	return log;
}



GLuint createAddAttribBuffer(GLuint vertexArrayObject, const void *data, const size_t dataSize, GLuint attributeIndex, GLsizei attributeSize, GLenum type, GLenum bufferUsage)
{
	GLuint buffer = 0;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, dataSize, data, bufferUsage);
	CHECK_GL_ERROR();

	// Now attach buffer to vertex array object.
	glBindVertexArray(vertexArrayObject);
	glVertexAttribPointer(attributeIndex, attributeSize, type, false, 0, 0 );	
	glEnableVertexAttribArray(attributeIndex);
	CHECK_GL_ERROR();

	return buffer;
}


#if defined(__linux__)
void linux_initialize_cwd()
{
	const unsigned kMaxPath = 1024; // Oh, well.

	/* Attempt to change CWD (current working directory) to 
	 * 		- ${BINDIR}/../share/${BINNAME}/
	 * 		- ${BINDIR}
	 *	where ${BINDIR} refers to the path of the directory where the binary
	 *	is located, and ${BINNAME} refers to the name of the binary.
	 *
	 *	For example, if we launch `bin/lab2' in `/home/foo/labs':
	 * 		$ cd /home/foo/labs
	 * 		$ bin/lab2
	 *	we'll try to change to 
	 *		- /home/foo/labs/share/lab2
	 *		- /home/foo/labs/bin
	 *	(in this case, it's assumed that the former succeeds.)
	 */
	
	// try to get the path to the binary
	char pathToBin[kMaxPath];
	ssize_t ret = readlink( "/proc/self/exe", pathToBin, kMaxPath-1 );

	if( ret == -1 )
		return;

	pathToBin[ret] = '\0';

	// extract parts
	char* dirsep = strrchr( pathToBin, '/' );
	if( dirsep ) *dirsep = '\0';

	const char* bindir = pathToBin;
	const char* binname = dirsep + 1;

	// attempt first ${BINDIR}/../share/${BINNAME}
	char pathToData[kMaxPath];
	snprintf( pathToData, kMaxPath, "%s/../share/%s", bindir, binname );

	if( 0 == chdir( pathToData ) )
	{
		fprintf( stderr, "Note: expecting to find data in `%s'\n", pathToData );
		return;
	}

	// then attempt ${BINDIR}
	if( 0 == chdir( pathToBin ) )
	{
		fprintf( stderr, "Note: expecting to find data in `%s'\n", pathToBin );
		return;
	}

	// neither seems to exist; give up.
	char currentCWD[kMaxPath];
	if( !getcwd( currentCWD, kMaxPath ) )
		strncpy( currentCWD, "???", kMaxPath );

	fprintf( stderr, "Note: expecting to find data in `%s'\n", currentCWD );
	return;
}

#endif // ! __linux__
