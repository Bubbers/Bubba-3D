
#include <ImageTexture.h>
#include <stb_image.h>
#include <Logger.h>
#include <glutil/glutil.h>

ImageTexture::ImageTexture(std::string fileName) {
    this->fileName = fileName;
}

void ImageTexture::loadTexture() {
    stbi_set_flip_vertically_on_load(true);

    int comp;
    unsigned char* image = stbi_load(fileName.c_str(), &width, &height, &comp, 0);

    if(image == nullptr) {
        Logger::logError("Couldnt load image "+ fileName);
    }

    GLuint texid;
    glGenTextures(1, &texid);
    glActiveTexture(GL_TEXTURE0);
    CHECK_GL_ERROR();
    glBindTexture(GL_TEXTURE_2D, texid);
    CHECK_GL_ERROR();


    GLenum format = comp == 3 ? GL_RGB : GL_RGBA;

    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA_EXT,
                 width, height, 0, format, GL_UNSIGNED_BYTE, image);

    CHECK_GL_ERROR();
    glGenerateMipmap(GL_TEXTURE_2D);
    CHECK_GL_ERROR();
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
    CHECK_GL_ERROR();

    glBindTexture(GL_TEXTURE_2D, 0);
    CHECK_GL_ERROR();
    textureID = texid;

    stbi_image_free(image);
    Logger::logInfo("Loaded image: " + fileName);

}

