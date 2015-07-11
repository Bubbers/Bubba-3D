#ifndef __SCENE_H__
#define __SCENE_H__

#define MAX_POINT_LIGHTS 2

#include <Mesh.h>
#include <Skybox.h>
#include <CubeMapTexture.h>
#include "Utils.h"
#include "Lights.h"


class Scene
{
public:
	Scene();
	~Scene();

	Skybox *skybox;
	Mesh *car;
	CubeMapTexture *cubeMap;

	Camera *shadowMapCamera; //Will follow directional light
	DirectionalLight directionalLight;
	std::vector<PointLight> pointLights;
	std::vector<SpotLight> spotLights;
	std::vector<Mesh*> lights;
	std::vector<Mesh*> shadowCasters;
	std::vector<Mesh*> transparentObjects;
};

#endif // __SCENE_H__