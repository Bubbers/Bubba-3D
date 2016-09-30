#pragma once

#include <IHudDrawable.h>
#include <GL/glew.h>
#include <vector>

/**
 * A low-level OpenGL object that renders text within the specified dimensions.
 */
class TextObject : public IHudDrawable {

public:
    /**
     * Creates a text object that should be rendered at position (x,y). The
     * coordinates are screen-type 2D coordinates when used with the HudRenderer (see HudRenderer).
     */
    TextObject(std::string text, Font* font, int width, int height, int x, int y);

    ~TextObject() = default;
    /**
     * Renders the object using the specified shader program and projection
     * matrix. The shader should have the same variables as the default hud.vert
     * and hud.frag.
     */
    void virtual render(ShaderProgram* shaderProgram, chag::float4x4* projectionMatrix);

    /**
     * Changes the text of the TextObject.
     */
    void virtual setText(std::string text);

protected:
    std::string text;
    Font* font;
    int width;
    int height;
    int x;
    int y;
    int numVertices;

    GLuint vao, vbo;
    bool buffersInitiated = false;

private:
    void addPoints(std::vector<GLfloat> *data, std::initializer_list<float> elems);
    void getLines(std::vector<std::string>* lines, std::vector<int>* linesOffsetLeft, int* numChars);
    int getOffsetByLineLength(int lineLength);
    void init(std::vector<std::string> lines, std::vector<int> linesOffsetLeft, int numChars);
    chag::float4x4 getModelMatrix();
    void initAndBindBuffers();

};
