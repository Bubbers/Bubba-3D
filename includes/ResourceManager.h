#ifndef BUBBA_3D_RESOURCEMANAGER_H
#define BUBBA_3D_RESOURCEMANAGER_H

#include <map>
#include "Shader.h"

class ResourceManager {
public:
    static void loadShader(const std::string &vertexShader, const std::string &fragmentShader, std::string name);
    static Shader getShader(std::string name);

private:
    static std::map<std::string, Shader> shaders;

};

#endif //BUBBA_3D_RESOURCEMANAGER_H
