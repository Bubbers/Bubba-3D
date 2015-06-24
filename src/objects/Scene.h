#ifndef __SCENE_H__
#define __SCENE_H__

#define MAX_POINT_LIGHTS 2

#include <Mesh.h>
#include <Skybox.h>
#include <CubeMapTexture.h>
#include "..\Misc\Utils.h"
#include "lights\Lights.h"


class Scene
{
public:
	Scene();
	~Scene();

	Skybox *skybox;
	Mesh *car;
	Fbo *cubeMap;

	Camera *shadowMapCamera; //Will follow directional light
	DirectionalLight directionalLight;
	std::vector<PointLight> pointLight;
	std::vector<Mesh*> lights;
	std::vector<Mesh*> shadowCasters;
	std::vector<Mesh*> transparentObjects;
};

#endif // __SCENE_H__