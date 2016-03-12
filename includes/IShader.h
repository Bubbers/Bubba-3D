//
// Created by johan on 2016-03-12.
//

#ifndef SUPER_BUBBA_AWESOME_SPACE_ISHADER_H
#define SUPER_BUBBA_AWESOME_SPACE_ISHADER_H

#include <string>
#include "Logger.h"
#include "glutil/glutil.h"

class IShader {
public:
    virtual void compile() = 0;
    virtual void checkErrors() = 0;

    GLuint compileShader(GLenum type, const char *source) {
        GLuint compiledShader = glCreateShader(type);
        glShaderSource(compiledShader, 1, &source, NULL);
        glCompileShader(compiledShader);
        return compiledShader;
    }

    void logCompileError(GLuint shader, std::string shaderType) {
        GLchar compileLog[MAX_LOG_SIZE];

        glGetShaderInfoLog(shader, MAX_LOG_SIZE, NULL, compileLog);
        Logger::logError("Failed to compile shader of type " + shaderType + "\n" + compileLog);
    }

    void checkCompileErrors(GLuint *shader, std::string shaderType) {
        GLint successfullyCompiled = false;
        glGetShaderiv(*shader, GL_COMPILE_STATUS, &successfullyCompiled);
        if (!successfullyCompiled) {
            logCompileError(*shader, shaderType);
        }
    }


}


};

#endif //SUPER_BUBBA_AWESOME_SPACE_ISHADER_H
