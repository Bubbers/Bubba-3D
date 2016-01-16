#ifndef BUBBA_3D_RESOURCEMANAGER_H
#define BUBBA_3D_RESOURCEMANAGER_H

#include <map>
#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"
#include <sstream>

class ResourceManager {
public:
    static void loadShader(const std::string &vertexShader, const std::string &fragmentShader, std::string name);
    static Shader* getShader(std::string name);

    static Texture* loadAndFetchTexture(const std::string &fileName);
    static Mesh*    loadAndFetchMesh   (const std::string &fileName);

    template<typename Type>
    static Type* getItemFromMap(std::map<std::string, Type> *map, std::string id) ;

private:
    static std::map<std::string, Shader> shaders;
    static std::map<std::string, Texture> textures;
    static std::map<std::string, Mesh> meshes;

    static void loadTexture(const std::string &fileName);
    static Texture* getTexture(std::string fileName);

    static void loadMesh(const std::string &fileName);
    static Mesh* getMesh(std::string fileName);

};

#endif //BUBBA_3D_RESOURCEMANAGER_H
