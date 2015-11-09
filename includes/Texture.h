#ifndef BUBBA_3D_TEXTURE_H
#define BUBBA_3D_TEXTURE_H

#include <GL/glew.h>


class Texture {
public:
    void bind(GLenum textureUnit);
    void loadTexture(std::string fileName);
private:
    GLuint textureID;
};

#endif //BUBBA_3D_TEXTURE_H
