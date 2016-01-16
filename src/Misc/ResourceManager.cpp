#include "ResourceManager.h"
#include <sstream>
#include <Logger.h>


std::map<std::string, Shader> ResourceManager::shaders;
std::map<std::string, Texture> ResourceManager::textures;
std::map<std::string, Mesh> ResourceManager::meshes;
std::map<std::string, sf::Music*> ResourceManager::musics;
std::map<std::string, sf::SoundBuffer> ResourceManager::soundBuffers;

void ResourceManager::loadShader(const std::string &vertexShader, const std::string &fragmentShader, std::string name){
    Shader shaderProgram;
    shaderProgram.loadShader(vertexShader,fragmentShader);
    shaders.insert(std::pair<std::string, Shader>(name, shaderProgram));
}

Shader* ResourceManager::getShader(std::string name) {
    return getItemFromMap(&shaders, name);
}

Texture* ResourceManager::loadAndFetchTexture(const std::string &fileName) {
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

Texture* ResourceManager::getTexture(std::string fileName) {
    return getItemFromMap(&textures, fileName);
}


Mesh* ResourceManager::loadAndFetchMesh(const std::string &fileName){
    try {
        return getMesh(fileName);
    } catch (std::invalid_argument exception) {
        loadMesh(fileName);
        return getMesh(fileName);
    }
}

void ResourceManager::loadMesh(const std::string &fileName){
    Mesh mesh;
    mesh.loadMesh(fileName);
    meshes.insert(std::pair<std::string, Mesh>(fileName, mesh));
}

Mesh* ResourceManager::getMesh(std::string fileName)
{
    return getItemFromMap(&meshes, fileName);
}


sf::Sound* ResourceManager::loadAndFetchSound(const std::string &fileName){
    try {
        return getSoundBuffer(fileName);
    } catch (std::invalid_argument exception) {
        loadSoundBuffer(fileName);
        return getSoundBuffer(fileName);
    }
}

void ResourceManager::loadSoundBuffer(const std::string &fileName) {
    sf::SoundBuffer soundBuffer;
    if(soundBuffer.loadFromFile(fileName) == -1) {
        Logger::logSevere("Unable to load soundbuffer " + fileName);
    }

    soundBuffers.insert(std::pair<std::string, sf::SoundBuffer>(fileName, soundBuffer));
}

sf::Sound* ResourceManager::getSoundBuffer(std::string fileName){
    sf::SoundBuffer *soundBuffer = getItemFromMap(&soundBuffers, fileName);
    sf::Sound *sound = new sf::Sound();
    sound->setBuffer(*soundBuffer);
    return sound;
}


sf::Music* ResourceManager::loadAndFetchMusic(const std::string &fileName) {
    try {
        return getMusic(fileName);
    } catch (std::invalid_argument exception) {
        loadMusic(fileName);
        return getMusic(fileName);
    }
}

void ResourceManager::loadMusic(const std::string &fileName) {
    sf::Music *music = new sf::Music();
    if(music->openFromFile(fileName) == -1){
        Logger::logSevere("Unable to load music " + fileName);
    }

    musics.insert(std::pair<std::string, sf::Music*>(fileName, music));
}

sf::Music* ResourceManager::getMusic(std::string fileName) {
    return *getItemFromMap(&musics, fileName);
}

template<typename Type>
Type* ResourceManager::getItemFromMap(std::map<std::string, Type> *map, std::string id) {
    typename std::map<std::string, Type>::iterator it = map->find(id);
    if( it != map->end()) {
        return &it->second;
    } else {
        std::stringstream errorMessage;
        errorMessage << id << " hasn't been loaded into ResourceManager before fetched";
        throw std::invalid_argument(errorMessage.str());
    }
}






