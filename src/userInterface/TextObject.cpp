//
// Created by simon on 2016-03-12.
//

#include <Font.h>
#include <Globals.h>
#include <GL/glew.h>
#include <glutil/glutil.h>
#include "TextObject.h"
#include <Shader.h>
#include <FontManager.h>
#include <ResourceManager.h>

TextObject::TextObject(string text, Font* font, int width, int height, int x, int y)
        : text(text), font(font), width(width), height(height), x(x), y(-y){
    init();
}

void TextObject::init() {

    int x = this->x;
    int y = this->y;
    GLfloat data[text.size()*5*6];
    int i = 0;
    float atlasWidth = Globals::get(Globals::FONT_TEXTURE_WIDTH),
            atlasHeight = Globals::get(Globals::FONT_TEXTURE_HEIGHT);
    vector<float> word;
    Font::GlyphData gData;
    for(char c : text){
        if(c >= 32){
            gData = font->getCharacter(c);
            float x2 =  x + gData.bitmapLeft;
            float y2 = -y - gData.bitmapTop;
            float w = gData.bitmapWidth;
            float h = gData.bitmapHeight;
            float ox = gData.offsetX;

            /* Advance the cursor to the start of the next character */
            x += gData.advanceX/64;
            y += gData.advanceY/64;

            /* Skip glyphs that have no pixels */
            if(!w || !h)
                continue;

            addPoints(data,i,{x2+w   , -y2    , 0, (ox+w)/atlasWidth, 0});
            i += 5;
            addPoints(data,i,{x2   , -y2 , 0,  ox/atlasWidth      , 0});
            i += 5;
            addPoints(data,i,{x2, -y2-h   , 0, ox/atlasWidth      , h/atlasHeight});
            i += 5;
            addPoints(data,i,{x2 + w, -y2, 0, (ox+w)/atlasWidth, 0});
            i += 5;
            addPoints(data,i,{x2, -y2-h  , 0, ox/atlasWidth, h/atlasHeight});
            i += 5;
            addPoints(data,i,{x2 + w, -y2 -h, 0, (ox+w)/atlasWidth      , h/atlasHeight});
            i += 5;
        }
    }
    numVertices = i/5;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint pos_vbo;
    glGenBuffers(1, &pos_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, pos_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

    // CLEANUP
    glBindVertexArray(0);
    CHECK_GL_ERROR();

}

void TextObject::addPoints(float *data, int i, initializer_list<float> elems) {

    for(float elem : elems)
        data[i++] = elem;

}

void TextObject::render(Shader *shaderProgram, float4x4 *projectionMatrix) {

    GLint currentDepthFunc;
    glGetIntegerv(GL_DEPTH_FUNC, &currentDepthFunc);

    glDepthFunc(GL_ALWAYS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shaderProgram->backupCurrentShaderProgram();

    shaderProgram->use();
    glBindVertexArray(vao);

    shaderProgram->setUniform1i("sprite", 1);
    shaderProgram->setUniformMatrix4fv("modelMatrix", getModelMatrix());
    shaderProgram->setUniformMatrix4fv("projectionMatrix",*projectionMatrix);
    shaderProgram->setUniform1i("isTexture",false);
    shaderProgram->setUniform1i("isColor",false);
    shaderProgram->setUniform1i("isFont",true);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,*FontManager::getTex());

    glDrawArrays(GL_TRIANGLES, 0, numVertices);

    CHECK_GL_ERROR();

    glDepthFunc(currentDepthFunc);
    glDisable(GL_BLEND);

    glEnable(GL_CULL_FACE);
    shaderProgram->restorePreviousShaderProgram();

}

float4x4 TextObject::getModelMatrix() {
    return make_translation(relativePosition)*make_rotation_z<float4x4>(rotation)*make_translation(center)*make_identity<float4x4>();
}