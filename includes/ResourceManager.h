#ifndef BUBBA_3D_RESOURCEMANAGER_H
#define BUBBA_3D_RESOURCEMANAGER_H

#include <map>
#include "Shader.h"
#include "Texture.h"

class ResourceManager {
public:
    static void loadShader(const std::string &vertexShader, const std::string &fragmentShader, std::string name);
    static Shader getShader(std::string name);

    static Texture loadAndFetch(const std::string &fileName);
    static void loadTexture(const std::string &fileName);
    static Texture getTexture(std::string fileName);
private:
    static std::map<std::string, Shader> shaders;
    static std::map<std::string, Texture> textures;

};

#endif //BUBBA_3D_RESOURCEMANAGER_H
