
#ifndef HALLSOFBUBBA_IMAGETEXTURE_H
#define HALLSOFBUBBA_IMAGETEXTURE_H

#endif //HALLSOFBUBBA_IMAGETEXTURE_H

#include <Texture.h>

class ImageTexture: public Texture {
public:
    ImageTexture(std::string fileName);

    void loadTexture() override;

private:
    std::string fileName;
};