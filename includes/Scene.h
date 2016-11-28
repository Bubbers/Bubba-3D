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

/**
 * @brief Scene handles all objects and cameras in a game scene.
 *
 */
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

    /**
     * @brief registers a GameObject as a shadow caster
     *
     * Shadow casters are non transparent objects in the scene that cast shadows.
     * This method adds a GameObject to the list of shadow casters in this scene.
     *
     * @param object The object that will be added.
     */
    void addShadowCaster(std::shared_ptr<GameObject> object);

    /**
     * @brief registers a GameObject as a transparent object
     *
     * This method adds a GameObject to the list of transparent objects in this scene.
     * Transparent objects does not cast shadows in the scene.
     *
     * @param object The object that will be added.
     */
    void addTransparentObject(std::shared_ptr<GameObject> object);

    std::vector<std::shared_ptr<GameObject>> getGameObjects();
    std::vector<std::shared_ptr<GameObject>> getShadowCasters();
    std::vector<std::shared_ptr<GameObject>> getTransparentObjects();

    virtual void update(float dt, std::vector<std::shared_ptr<GameObject>> *toDelete);

private:
    std::vector<std::shared_ptr<GameObject>> shadowCasters;
    std::vector<std::shared_ptr<GameObject>> transparentObjects;
    std::vector<std::shared_ptr<GameObject>> allObjects;

    void removeDirty(std::vector<std::shared_ptr<GameObject>> *v,
                     std::vector<std::shared_ptr<GameObject>> *toDelete);
};
