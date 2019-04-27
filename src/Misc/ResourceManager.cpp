/*
 * This file is part of Bubba-3D.
 *
 * Bubba-3D is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Bubba-3D is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Bubba-3D. If not, see http://www.gnu.org/licenses/.
 */
#include "ResourceManager.h"
#include <sstream>
#include <Logger.h>
#include <ShaderProgram.h>
#include <shader/VertexShader.h>
#include <shader/FragmentShader.h>
#include "objects/Chunk.h"
#include "Texture.h"
#include "Mesh.h"
#include <sstream>
#include <ImageTexture.h>

std::map<std::string, std::shared_ptr<ShaderProgram>> ResourceManager::shaders;
std::map<std::string, std::shared_ptr<Texture>> ResourceManager::textures;
std::map<std::string, std::shared_ptr<IMesh>> ResourceManager::meshes;

#ifdef __linux__
std::map<std::string, std::shared_ptr<sf::Music>> ResourceManager::musics;
std::map<std::string, std::shared_ptr<sf::SoundBuffer>> ResourceManager::soundBuffers;

FileWatcher ResourceManager::fileWatcher = FileWatcher();
#endif

std::shared_ptr<ShaderProgram> ResourceManager::loadAndFetchShaderProgram(
    const std::string &shaderName,
    const std::string &vertexShader,
    const std::string &fragmentShader)
{
    try {
        return getShader(shaderName);
    } catch (std::invalid_argument exception) {
        loadShader(vertexShader, fragmentShader, shaderName);
        return getShader(shaderName);
    }
}

std::shared_ptr<Texture> ResourceManager::loadAndFetchTexture(const std::string &fileName) {
    try {
        return getTexture(fileName);
    } catch (std::invalid_argument exception) {
        loadTexture(fileName);
        return getTexture(fileName);
    }
}

std::shared_ptr<IMesh> ResourceManager::loadAndFetchMesh(const std::string &fileName) {
    try {
        return getMesh(fileName);
    } catch (std::invalid_argument exception) {
        loadMesh(fileName);
        return getMesh(fileName);
    }
}

void ResourceManager::loadShader(const std::string &vertexShader,
                                 const std::string &fragmentShader,
                                 const std::string &name)
{
    std::shared_ptr<ShaderProgram> shaderProgram = std::make_shared<ShaderProgram>();
    shaderProgram->loadShader(std::make_shared<VertexShader>(vertexShader), std::make_shared<FragmentShader>(fragmentShader));

#ifdef __linux__
    ResourceManager::fileWatcher.addWatch(vertexShader, [vertexShader, fragmentShader, shaderProgram](){
        Logger::logInfo("Reloading shader");
        shaderProgram->loadShader(std::make_shared<VertexShader>(vertexShader), std::make_shared<FragmentShader>(fragmentShader));
    });
    ResourceManager::fileWatcher.addWatch(fragmentShader, [vertexShader, fragmentShader, shaderProgram](){
        Logger::logInfo("Reloading shader");
        shaderProgram->loadShader(std::make_shared<VertexShader>(vertexShader), std::make_shared<FragmentShader>(fragmentShader));
    });
#endif

    shaders.insert(std::pair<std::string, std::shared_ptr<ShaderProgram>>
                            (name, std::move(shaderProgram)));
}

void ResourceManager::loadTexture(const std::string &fileName) {
    std::shared_ptr<ImageTexture> texture = std::make_shared<ImageTexture>(fileName);
    texture->loadTexture();

#ifdef __linux__
    ResourceManager::fileWatcher.addWatch(fileName, [fileName, texture](){
        texture->loadTexture();
    });
#endif

    textures.insert(std::pair<std::string, std::shared_ptr<Texture>>(fileName, std::move(texture)));
}

void ResourceManager::loadMesh(const std::string &fileName) {
    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
    mesh->loadMesh(fileName);

#ifdef __linux__
    ResourceManager::fileWatcher.addWatch(fileName, [fileName, mesh](){
        mesh->loadMesh(fileName);
    });
#endif


    meshes.insert(std::pair<std::string, std::shared_ptr<Mesh>>(fileName, std::move(mesh)));
}

std::shared_ptr<ShaderProgram> ResourceManager::getShader(const std::string &name) {
    return getItemFromMap(shaders, name);
}

std::shared_ptr<Texture> ResourceManager::getTexture(const std::string &fileName) {
    return getItemFromMap(textures, fileName);
}


std::shared_ptr<IMesh> ResourceManager::getMesh(const std::string &fileName) {
    return getItemFromMap(meshes, fileName);
}

template<typename Type>
std::shared_ptr<Type> ResourceManager::getItemFromMap(
    std::map<std::string, std::shared_ptr<Type>> &map,
    const std::string &id)
{
    typename std::map<std::string, std::shared_ptr<Type>>::iterator it = map.find(id);
    if( it != map.end()) {
        return it->second;
    } else {
        std::stringstream errorMessage;
        errorMessage << id << " hasn't been loaded into ResourceManager before fetched";
        throw std::invalid_argument(errorMessage.str());
    }
}

#ifdef __linux__
std::shared_ptr<sf::Sound> ResourceManager::loadAndFetchSound(const std::string &fileName){
    try {
        return getSoundBuffer(fileName);
    } catch (std::invalid_argument exception) {
        loadSoundBuffer(fileName);
        return getSoundBuffer(fileName);
    }
}

void ResourceManager::loadSoundBuffer(const std::string &fileName) {
    std::shared_ptr<sf::SoundBuffer> soundBuffer = std::make_shared<sf::SoundBuffer>();
    soundBuffer->loadFromFile(fileName);

    ResourceManager::fileWatcher.addWatch(fileName, [fileName, soundBuffer](){
        const std::string fileNameConst = fileName;
        soundBuffer->loadFromFile(fileNameConst);
    });

    soundBuffers.insert(std::pair<std::string, std::shared_ptr<sf::SoundBuffer>>(fileName, soundBuffer));
}

std::shared_ptr<sf::Sound> ResourceManager::getSoundBuffer(std::string fileName){
    std::shared_ptr<sf::SoundBuffer> soundBuffer = getItemFromMap(soundBuffers, fileName);
    std::shared_ptr<sf::Sound> sound = std::make_shared<sf::Sound>();
    sound->setBuffer(*soundBuffer);
    return sound;
}


std::shared_ptr<sf::Music> ResourceManager::loadAndFetchMusic(const std::string &fileName) {
    try {
        return getMusic(fileName);
    } catch (std::invalid_argument exception) {
        loadMusic(fileName);
        return getMusic(fileName);
    }
}

void ResourceManager::loadMusic(const std::string &fileName) {
    std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>();
    music->openFromFile(fileName);

    ResourceManager::fileWatcher.addWatch(fileName, [fileName, music](){
        music->openFromFile(fileName);
    });

    musics.insert(std::pair<std::string, std::shared_ptr<sf::Music>>(fileName, music));
}

std::shared_ptr<sf::Music> ResourceManager::getMusic(std::string fileName) {
    return getItemFromMap(musics, fileName);
}

void ResourceManager::update() {
    ResourceManager::fileWatcher.update();
}

#endif
