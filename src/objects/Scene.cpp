#include "Scene.h"


Scene::Scene()
{
}


Scene::~Scene()
{
}


void Scene::update(float dt) {
    for(auto &object : shadowCasters ) {
        object->update(dt);
    }
}