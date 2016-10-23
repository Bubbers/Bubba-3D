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
#pragma once

#include <map>
#include <sstream>
#include <memory>

class Texture;
class Mesh;
class ShaderProgram;

/**
 * @brief ResourceManager for all resources that can be shared 
 *
 * The purpose of this class is to only maintain one copy of
 * each resource in memory and then distribute them as shared pointers.
 *
 */
class ResourceManager {
public:

    /**
     * @brief Tries to load a Texture into the ResourceManager and then return it.
     *
     * Tries to fetch a Texture from the ResourceManager.
     * If the Texture is not already loaded it is loaded before being fetched.
     *
     * @param shaderName The name of the shader
     * @param vertexShader The name of the vertexshader file.
     * @param fragmentShader The name of the fragmenshare file.
     */
    static std::shared_ptr<ShaderProgram> loadAndFetchShaderProgram(const std::string &name,
                                                                    const std::string &vertexShader,
                                                                    const std::string &fragmentShader);

    /**
     * @brief Tries to load a Texture into the ResourceManager and then return it.
     *
     * Tries to fetch a Texture from the ResourceManager.
     * If the Texture is not already loaded it is loaded before being fetched.
     *
     * @param fileName The name of the texture file.
     */
    static std::shared_ptr<Texture> loadAndFetchTexture(const std::string &fileName);

    /**
     * @brief Tries to load a Mesh into the ResourceManager and then return it.
     *
     * Tries to fetch a Mesh from the ResourceManager.
     * If the Mesh is not already loaded it is loaded before being fetched.
     *
     * @param fileName The name of the mesh file.
     */
    static std::shared_ptr<Mesh>    loadAndFetchMesh   (const std::string &fileName);


private:
    static std::map<std::string, std::shared_ptr<ShaderProgram>> shaders;
    static std::map<std::string, std::shared_ptr<Texture>> textures;
    static std::map<std::string, std::shared_ptr<Mesh>> meshes;

    /**
     * @brief Loads a ShaderProgram into the ResourceManager
     */
    static void loadShader(const std::string &vertexShader,
                           const std::string &fragmentShader,
                           const std::string &name);

    /**
     * @brief Gets a already loaded ShaderProgram from the ResourceManager 
     */
    static std::shared_ptr<ShaderProgram> getShader(const std::string &name);

    static void loadTexture(const std::string &fileName);
    static std::shared_ptr<Texture> getTexture(const std::string &fileName);

    static void loadMesh(const std::string &fileName);
    static std::shared_ptr<Mesh> getMesh(const std::string &fileName);

    template<typename Type>
    static std::shared_ptr<Type> getItemFromMap(std::map<std::string, std::shared_ptr<Type>> &map,
                                                const std::string &id) ;

};
