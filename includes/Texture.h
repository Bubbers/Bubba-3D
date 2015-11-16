#ifndef BUBBA_3D_TEXTURE_H
#define BUBBA_3D_TEXTURE_H

#include <GL/glew.h>
#include <FreeImage.h>


class Texture {
public:
    Texture() {};
    Texture(GLuint textureID) {this->textureID = textureID; };
    void bind(GLenum textureUnit);
    void loadTexture(std::string fileName);
    GLuint getID() {return textureID; };
    static FIBITMAP *LoadImageIntoMemory(std::string fileName);
private:
    GLuint textureID;
};

#endif //BUBBA_3D_TEXTURE_H
