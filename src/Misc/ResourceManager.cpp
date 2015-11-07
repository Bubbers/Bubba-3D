#include "ResourceManager.h"
#include <sstream>


std::map<std::string, Shader> ResourceManager::shaders;

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

