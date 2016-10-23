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

std::map<std::string, std::shared_ptr<ShaderProgram>> ResourceManager::shaders;
std::map<std::string, std::shared_ptr<Texture>> ResourceManager::textures;
std::map<std::string, std::shared_ptr<Mesh>> ResourceManager::meshes;

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

std::shared_ptr<Mesh> ResourceManager::loadAndFetchMesh(const std::string &fileName) {
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
    shaderProgram->loadShader(new VertexShader(vertexShader), new FragmentShader(fragmentShader));
    shaders.insert(std::pair<std::string, std::shared_ptr<ShaderProgram>>
                            (name, std::move(shaderProgram)));
}

void ResourceManager::loadTexture(const std::string &fileName) {
    std::shared_ptr<Texture> texture = std::make_shared<Texture>();
    texture->loadTexture(fileName);
    textures.insert(std::pair<std::string, std::shared_ptr<Texture>>(fileName, std::move(texture)));
}

void ResourceManager::loadMesh(const std::string &fileName) {
    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
    mesh->loadMesh(fileName);
    meshes.insert(std::pair<std::string, std::shared_ptr<Mesh>>(fileName, std::move(mesh)));
}

std::shared_ptr<ShaderProgram> ResourceManager::getShader(const std::string &name) {
    return getItemFromMap(shaders, name);
}

std::shared_ptr<Texture> ResourceManager::getTexture(const std::string &fileName) {
    return getItemFromMap(textures, fileName);
}


std::shared_ptr<Mesh> ResourceManager::getMesh(const std::string &fileName) {
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

