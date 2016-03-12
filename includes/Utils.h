#ifndef __UTILS_H__
#define __UTILS_H__

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "ShaderProgram.h"

//*****************************************************************************
//	Frame buffer objects
//*****************************************************************************
struct Fbo{
	GLuint id;
	GLuint texture;
	ShaderProgram* shaderProgram;
	GLuint depthbuffer;

	int width, height;
};



static float degreeToRad(float degree) {
	return (float) (degree * M_PI / 180);
}

static float radToDegree(float rad) {
	return (float) (rad * 180 / M_PI);
}

static float getRand(float min, float max) {
	//srand(time(NULL));
	float range = max - min;
	return (((float) rand() / (float) RAND_MAX) * range) + min;
}

template <typename T, unsigned S>
static T getRandomElem(const T (&ts)[S])
{
	if (S > 0) {
		int ix = (int) getRand(0.0f, S - 0.1f);
		return ts[ix];
	} else {
		return NULL;
	}
}

static float3 createRandomVector(float minValue, float maxValue){
	return make_vector(getRand(minValue,maxValue),getRand(minValue,maxValue),getRand(minValue,maxValue));
}

enum State{Playing, Died, Start, Won, Credits};

    const char *textFileRead(const char *fn, bool fatalError) {

        FILE *fp;
        char *content = NULL;
        int count = 0;

        if (fn != NULL) {
            fp = fopen(fn, "rt");
            if (fp != NULL) {
                fseek(fp, 0, SEEK_END);
                count = ftell(fp);
                fseek(fp, 0, SEEK_SET);

                if (count > 0) {
                    content = new char[count+1];
                    count = fread( content, sizeof(char), count, fp);
                    content[count] = '\0';
                } else {
                    if (fatalError) {
                        char buffer[256];
                        sprintf(buffer, "File '%s' is empty\n", fn);
                        fatal_error(buffer);
                    }
                }

                fclose(fp);
            } else {
                if (fatalError) {
                    char buffer[256];
                    sprintf(buffer, "Unable to read file '%s'\n", fn);
                    fatal_error(buffer);
                }
            }
        } else {
            if (fatalError)
                fatal_error("textFileRead - argument NULL\n");
        }

        return content;
    }
#endif