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
#define MAX_POINT_LIGHTS 2

#include "Mesh.h"
#include "Utils.h"
#include "Lights.h"
#include "IDrawable.h"

class CubeMapTexture;
class Camera;
class GameObject;

class Scene
{
public:
    Scene();
    ~Scene();

    CubeMapTexture *cubeMap = nullptr;

    Camera *shadowMapCamera = nullptr; //Will follow directional light
    DirectionalLight directionalLight;
    std::vector<PointLight> pointLights;
    std::vector<SpotLight> spotLights;

    void addShadowCaster(GameObject* object);
    void addTransparentObject(GameObject* object);

    std::vector<GameObject*> getGameObjects();
    std::vector<GameObject*> getShadowCasters();
    std::vector<GameObject*> getTransparentObjects();

    virtual void update(float dt, std::vector<GameObject*> *toDelete);

private:
    std::vector<GameObject*> shadowCasters;
    std::vector<GameObject*> transparentObjects;
    std::vector<GameObject*> allObjects;

    void removeDirty(std::vector<GameObject*> *v, std::vector<GameObject*> *toDelete);
};
