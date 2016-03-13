//
// Created by simon on 2016-03-12.
//

#include <Font.h>
#include <Globals.h>
#include <GL/glew.h>
#include <glutil/glutil.h>
#include "TextObject.h"
#include <ShaderProgram.h>
#include <FontManager.h>
#include <ResourceManager.h>

TextObject::TextObject(string text, Font* font, int width, int height, int x, int y)
        : text(text), font(font), width(width), height(height), x(x), y(-y){

    vector<string> lines;
    vector<int> linesOffsetX;
    int numChars =0;
    getLines(&lines,&linesOffsetX,&numChars);

    init(lines,linesOffsetX, numChars);
}

void TextObject::getLines(vector<string>* lines, vector<int>* linesOffsetX, int* numChars){
    int lineLength = 0;
    string curLine = "";
    string word = "";
    int wordLength = 0;
    for(unsigned char c : text){
        if(c > ' ' && c < 128){ // visible characters
            wordLength += font->getCharacter(c).advanceX/64;
            word += c;
            (*numChars)++;
        }else if(c == ' '){
            if(lineLength + wordLength > width){
                lines->push_back(curLine);
                linesOffsetX->push_back(getOffsetByLineLength(lineLength));
                curLine = word;
                lineLength = wordLength;
            }else{
                curLine += " " + word;
                lineLength += font->getCharacter(' ').advanceX/64 + wordLength;
            }
            word = "";
            wordLength = 0;
        } else if(c == '\r'){
            if(lineLength + wordLength > width){
                lines->push_back(curLine);
                linesOffsetX->push_back(getOffsetByLineLength(lineLength));
                curLine = word;
                lineLength = wordLength;
            }else{
                curLine += " " + word;
                lineLength += font->getCharacter(' ').advanceX/64 + wordLength;
            }
            lines->push_back(curLine);
            linesOffsetX->push_back(getOffsetByLineLength(lineLength));
            curLine = "";
            lineLength = 0;
            word = "";
            wordLength = 0;
        }
    }
    if(lineLength + wordLength > width){
        lines->push_back(curLine);
        linesOffsetX->push_back(getOffsetByLineLength(lineLength));
        curLine = word;
        lineLength = wordLength;
    }else{
        curLine += " " + word;
        lineLength += font->getCharacter(' ').advanceX/64 + wordLength;
    }
    lines->push_back(curLine);
    linesOffsetX->push_back(getOffsetByLineLength(lineLength));
}

int TextObject::getOffsetByLineLength(int lineLength) {
    return (width-lineLength)/2;
}

void TextObject::init(vector<string> lines, vector<int> linesOffsetX, int numChars) {

    int x = this->x;
    int y = this->y - font->getPixelSize();
	std::vector<GLfloat> data;
    //GLfloat data[numChars*5*6];
    int i = 0;
    float atlasWidth = Globals::get(Globals::FONT_TEXTURE_WIDTH),
            atlasHeight = Globals::get(Globals::FONT_TEXTURE_HEIGHT);
    Font::GlyphData gData;
    int l = 0;
    for(string line : lines) {
        int lox = linesOffsetX[l++];
        for (unsigned char c : line) {
            if (c >= 32) {
                gData = font->getCharacter(c);
                float x2 = x + gData.bitmapLeft + lox;
                float y2 = -y - gData.bitmapTop;
                float w = gData.bitmapWidth;
                float h = gData.bitmapHeight;
                float ox = gData.offsetX;

                /* Advance the cursor to the start of the next character */
                x += gData.advanceX / 64;
                y += gData.advanceY / 64;

                /* Skip glyphs that have no pixels */
                if (!w || !h)
                    continue;

                addPoints(&data, i, {x2 + w, -y2, 0, (ox + w) / atlasWidth, 0});
                i += 5;
                addPoints(&data, i, {x2, -y2, 0, ox / atlasWidth, 0});
                i += 5;
                addPoints(&data, i, {x2, -y2 - h, 0, ox / atlasWidth, h / atlasHeight});
                i += 5;
                addPoints(&data, i, {x2 + w, -y2, 0, (ox + w) / atlasWidth, 0});
                i += 5;
                addPoints(&data, i, {x2, -y2 - h, 0, ox / atlasWidth, h / atlasHeight});
                i += 5;
                addPoints(&data, i, {x2 + w, -y2 - h, 0, (ox + w) / atlasWidth, h / atlasHeight});
                i += 5;
            }
        }
        y -= font->getPixelSize();
        x = this->x;
    }
    numVertices = i/5;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint pos_vbo;
    glGenBuffers(1, &pos_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, pos_vbo);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), data.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

    // CLEANUP
    glBindVertexArray(0);
    CHECK_GL_ERROR();

}

void TextObject::addPoints(std::vector<GLfloat> *data, int i, initializer_list<float> elems) {

    for(float elem : elems)
        data->push_back(elem);

}

void TextObject::render(ShaderProgram *shaderProgram, float4x4 *projectionMatrix) {

    GLint currentDepthFunc;
    glGetIntegerv(GL_DEPTH_FUNC, &currentDepthFunc);

    glDepthFunc(GL_ALWAYS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shaderProgram->backupCurrentShaderProgram();

    shaderProgram->use();
    glBindVertexArray(vao);

    shaderProgram->setUniform1i("sprite", 4);
    shaderProgram->setUniformMatrix4fv("modelMatrix", getModelMatrix());
    shaderProgram->setUniformMatrix4fv("projectionMatrix",*projectionMatrix);
    shaderProgram->setUniform1i("isTexture",false);
    shaderProgram->setUniform1i("isColor",false);
    shaderProgram->setUniform1i("isFont",true);

    glActiveTexture(GL_TEXTURE4);
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