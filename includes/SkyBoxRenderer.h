#ifndef __SKYBOXRENDERER_H__
#define __SKYBOXRENDERER_H__

#include "IRenderComponent.h"
#include <string>

using namespace std;

class Camera;
class CubeMapTexture;
class Mesh;
struct Chunk;
class GameObject;

class SkyBoxRenderer : public IRenderComponent
{
public:
	SkyBoxRenderer(Camera* camera, Mesh* skyMesh, GameObject* gameObject);
	~SkyBoxRenderer();

	bool init(const std::string& posXFilename, const std::string& negXFilename, const std::string& posYFilename, const std::string& negYFilename, const std::string& posZFilename, const std::string& negZFilename);

	void renderShadow(Shader* shaderProgram) {};
	void render();
	void update(float dt);
private:
	void renderChunk(Chunk& chunk);

	Camera* m_camera;
	CubeMapTexture* m_pCubemap;
	Mesh *m_skyMesh;
	GameObject* gameObject;
};


#endif // !__SKYBOXRENDERER_H__
