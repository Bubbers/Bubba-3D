#ifndef __SCENE_H__
#define __SCENE_H__

#define MAX_POINT_LIGHTS 2

#include "Mesh.h"
#include "Skybox.h"
#include "CubeMapTexture.h"
#include "Utils.h"
#include "Lights.h"
#include "IDrawable.h"

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

	void update(float dt);
};

#endif // __SCENE_H__
