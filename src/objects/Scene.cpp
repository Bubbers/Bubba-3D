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
#include "Scene.h"
#include "GameObject.h"

Scene::Scene()
{
}


Scene::~Scene()
{
}

void Scene::addShadowCaster(std::shared_ptr<GameObject> object) {
    shadowCasters.push_back(object);
    allObjects.push_back(object);
}

std::vector<std::shared_ptr<GameObject>> Scene::getShadowCasters() {
    return shadowCasters;
}

void Scene::addTransparentObject(std::shared_ptr<GameObject> object){
    transparentObjects.push_back(object);
    allObjects.push_back(object);
}

std::vector<std::shared_ptr<GameObject>> Scene::getTransparentObjects() {
    return transparentObjects;
}

std::vector<std::shared_ptr<GameObject>> Scene::getGameObjects() {
    return allObjects;
}


void Scene::update(float dt) {
    removeDirty(&shadowCasters);
    removeDirty(&transparentObjects);
    removeDirty(&allObjects);

    auto sCasters = shadowCasters;
    auto tObjects = transparentObjects;

    for(auto &object : sCasters ) {
        object->update(dt);
    }

    for(auto &object : tObjects ) {
        object->update(dt);
    }
}

void Scene::removeDirty(std::vector<std::shared_ptr<GameObject>> *objectsInScene)
{
    for(auto i = objectsInScene->begin(); i < objectsInScene->end(); )
    {
        if((*i)->isDirty())
        {
            i = objectsInScene->erase(i);
        }
        else {
            i++;
        }
    }
}
