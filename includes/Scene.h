#ifndef __SCENE_H__
#define __SCENE_H__

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

	void update(float dt, std::vector<GameObject*> *toDelete);

private:
	std::vector<GameObject*> shadowCasters;
	std::vector<GameObject*> transparentObjects;
	std::vector<GameObject*> allObjects;

	void removeDirty(std::vector<GameObject*> *v, std::vector<GameObject*> *toDelete);
};

#endif // __SCENE_H__
