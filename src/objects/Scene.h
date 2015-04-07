#ifndef __SCENE_H__
#define __SCENE_H__

#include <Mesh.h>
#include <Skybox.h>



class Scene
{
public:
	Scene();
	~Scene();

	Skybox *skybox;
	Camera *sun;
	Mesh *car;
	std::vector<Mesh*> lights;
	std::vector<Mesh*> shadowCasters;
	std::vector<Mesh*> transparentObjects;
};

#endif // __SCENE_H__