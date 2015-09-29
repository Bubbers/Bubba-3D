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

	IDrawable *skybox;
	CubeMapTexture *cubeMap;

	Camera *shadowMapCamera; //Will follow directional light
	DirectionalLight directionalLight;
	std::vector<PointLight> pointLights;
	std::vector<SpotLight> spotLights;

	std::vector<IDrawable*> shadowCasters;
	std::vector<IDrawable*> transparentObjects;
};

#endif // __SCENE_H__
