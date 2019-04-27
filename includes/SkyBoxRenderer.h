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

#include "IRenderComponent.h"
#include <string>
#include <memory>

class Camera;
class CubeMapTexture;
class IMesh;
class Chunk;
class GameObject;

class SkyBoxRenderer : public IRenderComponent
{
public:
    SkyBoxRenderer(Camera* camera, IMesh* skyMesh, std::shared_ptr<GameObject> gameObject);
    ~SkyBoxRenderer();

    bool init(const std::string& posXFilename, const std::string& negXFilename,
              const std::string& posYFilename, const std::string& negYFilename,
              const std::string& posZFilename, const std::string& negZFilename);

    void renderShadow(std::shared_ptr<ShaderProgram> &shaderProgram) {};
    void render();
    void update(float dt);
private:
    void renderChunk(Chunk& chunk);

    Camera* m_camera;
    CubeMapTexture* m_pCubemap;
    IMesh *m_skyMesh;
    std::shared_ptr<GameObject> gameObject;
};
