#ifndef __SHADER_H__
#define __SHADER_H__

#include <map>
#include <string>
#include "glutil/glutil.h"
#include "linmath/float3.h"
#include "IShader.h"

using namespace chag;


/**
 * \brief Class for maintaining OpenGL shader programs.
 *
 * The class maintains information about where the shaders are saved
 * on the graphics card. It caches the uniform locations as they are used to
 * speed up the next call.
 */
class ShaderProgram {
 public:
    ShaderProgram();

    /**
     * Compiles, links and creates a program of the specified shaders.
     */
    void loadShader(IShader *vertexShader, IShader *fragmentShader);

    /**
     * Activates the shader on the graphics card
     */
    void use();

    /**
     * Saves the shader currently active on the graphics card.
     * This is useful if later calculations may depend on the
     * currently active shader. Then you can save it, perform this
     * shader objects calculation, and restore the previous shader.
     */
    void backupCurrentShaderProgram();
    /**
     * Restores a shader saved by backupCurrentShaderProgram();
     */
    void restorePreviousShaderProgram();

    void setUniform1i(std::string name, int value);
    void setUniform1f(std::string name, float value);
    void setUniform3f(std::string name, float3 value);
    void setUniformMatrix4fv(std::string name, float4x4 matrix);

    void setUniformBufferObjectBinding(std::string bufferName, int index);
    void initUniformBufferObject(std::string bufferName, int size, int index);
    void setUniformBufferSubData(std::string bufferName, int offset, int size, const GLvoid *data);

 private:

    void linkProgram();
    void checkLinkageErrors();
    void logLinkageError();

    void createProgram(GLuint vertexShader, GLuint fragmentShader);

    GLint getUniformLocation(std::string name);

    IShader* vertexShader;
    IShader* fragmentShader;

    GLuint shaderID;
    std::map<std::string, GLint> uniformLocations;
    GLint previousShaderProgram;

};

#endif //__SHADER__H
