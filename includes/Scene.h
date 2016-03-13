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

	CubeMapTexture *cubeMap;

	Camera *shadowMapCamera; //Will follow directional light
	DirectionalLight directionalLight;
	std::vector<PointLight> pointLights;
	std::vector<SpotLight> spotLights;

	std::vector<GameObject*> shadowCasters;
	std::vector<GameObject*> transparentObjects;

	void update(float dt, std::vector<GameObject*> *toDelete);

private:
	void removeDirty(std::vector<GameObject*> *v, std::vector<GameObject*> *toDelete);
};

#endif // __SCENE_H__
