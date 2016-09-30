#include "Scene.h"
#include "GameObject.h"

Scene::Scene()
{
}


Scene::~Scene()
{
}

void Scene::addShadowCaster(GameObject* object) {
    shadowCasters.push_back(object);
    allObjects.push_back(object);
}

std::vector<GameObject*> Scene::getShadowCasters() {
    return shadowCasters;
}

void Scene::addTransparentObject(GameObject* object){
    transparentObjects.push_back(object);
    allObjects.push_back(object);
}

std::vector<GameObject*> Scene::getTransparentObjects() {
    return transparentObjects;
}

std::vector<GameObject*> Scene::getGameObjects() {
    return allObjects;
}


void Scene::update(float dt, std::vector<GameObject*> *toDelete) {
    removeDirty(&shadowCasters, toDelete);
    removeDirty(&transparentObjects, toDelete);
    removeDirty(&allObjects, toDelete);

	auto sCasters = shadowCasters;
	auto tObjects = transparentObjects;

    for(auto &object : sCasters ) {
        object->update(dt);
    }

    for(auto &object : tObjects ) {
        object->update(dt);
    }
}

void Scene::removeDirty(std::vector<GameObject*> *v, std::vector<GameObject*> *toDelete) {
    for(auto i = v->begin(); i < v->end(); )
    {
        if((*i)->isDirty())
        {
            toDelete->push_back(*i);
            i = v->erase(i);
		}
		else {
			i++;
		}
    }
}