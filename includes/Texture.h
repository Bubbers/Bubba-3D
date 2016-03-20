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

    int getHeight();
    int getWidth();

private:

    GLuint textureID;
    int width,height;
};

#endif //BUBBA_3D_TEXTURE_H
