#ifndef __SCENE_H__
#define __SCENE_H__

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
	Camera *sun;
	Mesh *car;

	Fbo *cubeMap;
	PointLight pointLight;
	std::vector<Mesh*> lights;
	std::vector<Mesh*> shadowCasters;
	std::vector<Mesh*> transparentObjects;
};

#endif // __SCENE_H__