#ifndef BUBBA_3D_TEXTURE_H
#define BUBBA_3D_TEXTURE_H

#include <GL/glew.h>
#include <FreeImage.h>
#include <string>

using namespace std;

class Texture {
public:
    Texture() {};
    Texture(GLuint textureID) {this->textureID = textureID; };
    void bind(GLenum textureUnit);
    void loadTexture(std::string fileName);
    GLuint getID() {return textureID; };
    static FIBITMAP *loadImage(std::string fileName);
private:
    static FREE_IMAGE_FORMAT getImageFormat(std::string fileName);
    static bool isUnsupportedFormat(FREE_IMAGE_FORMAT *imageFormat, std::string fileName);
    static FIBITMAP* loadImage32Bit(FREE_IMAGE_FORMAT imageFormat, std::string fileName);
    static bool shouldFlip(FREE_IMAGE_FORMAT imageFormat);
    static FIBITMAP* convertImageTo32Bit(FIBITMAP *image);
    GLuint textureID;
};

#endif //BUBBA_3D_TEXTURE_H
