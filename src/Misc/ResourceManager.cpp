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

std::map<std::string, ShaderProgram> ResourceManager::shaders;
std::map<std::string, Texture> ResourceManager::textures;
std::map<std::string, Mesh> ResourceManager::meshes;

void ResourceManager::loadShader(const std::string &vertexShader, const std::string &fragmentShader, std::string name){
    ShaderProgram shaderProgram;
    shaderProgram.loadShader(new VertexShader(vertexShader), new FragmentShader(fragmentShader));
    shaders.insert(std::pair<std::string, ShaderProgram>(name, shaderProgram));
}

ShaderProgram* ResourceManager::getShader(std::string name) {
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






