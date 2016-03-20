//
// Created by simon on 2016-03-12.
//

#ifndef SUPER_BUBBA_AWESOME_SPACE_TEXTOBJECT_H
#define SUPER_BUBBA_AWESOME_SPACE_TEXTOBJECT_H

#include <IHudDrawable.h>
#include <GL/glew.h>
#include <vector>

using namespace std;
using namespace chag;

/**
 * A low-level OpenGL object that renders text within the specified dimensions.
 */
class TextObject : public IHudDrawable {

public:
    /**
     * Creates a text object that should be rendered at position (x,y). The
     * coordinates are screen-type 2D coordinates when used with the HudRenderer (see HudRenderer).
     */
    TextObject(string text, Font* font, int width, int height, int x, int y);

    /**
     * Renders the object using the specified shader program and projection
     * matrix. The shader should have the same variables as the default hud.vert
     * and hud.frag.
     */
    void virtual render(ShaderProgram* shaderProgram, float4x4* projectionMatrix);

    /**
     * Changes the text of the TextObject.
     */
    void virtual setText(string text);

protected:
    string text;
    Font* font;
    int height, width, x, y;
    int numVertices;

    GLuint vao, vbo;
    bool buffersInitiated = false;

private:
    void addPoints(std::vector<GLfloat> *data, initializer_list<float> elems);
    void getLines(vector<string>* lines, vector<int>* linesOffsetLeft, int* numChars);
    int getOffsetByLineLength(int lineLength);
    void init(vector<string> lines, vector<int> linesOffsetLeft, int numChars);
    float4x4 getModelMatrix();
    void initAndBindBuffers();

};


#endif //SUPER_BUBBA_AWESOME_SPACE_TEXTOBJECT_H
