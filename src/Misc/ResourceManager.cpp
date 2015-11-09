#include "ResourceManager.h"
#include <sstream>


std::map<std::string, Shader> ResourceManager::shaders;
std::map<std::string, Texture> ResourceManager::textures;

void ResourceManager::loadShader(const std::string &vertexShader, const std::string &fragmentShader, std::string name){
    Shader shaderProgram;
    shaderProgram.loadShader(vertexShader,fragmentShader);
    shaders.insert(std::pair<std::string, Shader>(name, shaderProgram));
}

Shader ResourceManager::getShader(std::string name) {
    std::map<std::string, Shader>::iterator it =  shaders.find(name);
    if( it != shaders.end()) {
        return it->second;
    } else {
        std::stringstream errorMessage;
        errorMessage << "Shader " << name << " hasn't been loaded into ResourceManager before fetched";
        throw std::invalid_argument(errorMessage.str());
    }
}

Texture ResourceManager::loadAndFetch(const std::string &fileName) {
    try {
        return getTexture(fileName);
    } catch (std::invalid_argument exception) {
        loadTexture(fileName);
        return getTexture(fileName);
    }
}

void ResourceManager::loadTexture(const std::string &fileName) {
    Texture texture;
    texture.loadTexture(fileName);
    textures.insert(std::pair<std::string, Texture>(fileName, texture));
}

Texture ResourceManager::getTexture(std::string fileName) {
    std::map<std::string, Texture>::iterator it =  textures.find(fileName);
    if( it != textures.end()) {
        return it->second;
    } else {
        std::stringstream errorMessage;
        errorMessage << "Shader " << fileName << " hasn't been loaded into ResourceManager before fetched";
        throw std::invalid_argument(errorMessage.str());
    }
}


