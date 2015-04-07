#ifndef __SCENE_H__
#define __SCENE_H__

#include <Mesh.h>

class Scene
{
public:
	Scene();
	~Scene();

	std::vector<Mesh*> lights;
	std::vector<Mesh*> shadowCasters;
	std::vector<Mesh*> transparentObjects;
};

#endif // __SCENE_H__