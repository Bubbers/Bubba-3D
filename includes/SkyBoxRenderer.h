#ifndef __SKYBOXRENDERER_H__
#define __SKYBOXRENDERER_H__


#include <string>
#include "Camera.h"
#include "CubeMapTexture.h"
#include "Mesh.h"
#include "IDrawable.h"
#include "GameObject.h"

using namespace std;

class SkyBoxRenderer : public IRenderComponent
{
public:
	SkyBoxRenderer(Camera* camera, Mesh* skyMesh, float4x4* modelMatrix);
	~SkyBoxRenderer();

	bool init(const string& posXFilename, const string& negXFilename, const string& posYFilename, const string& negYFilename, const string& posZFilename, const string& negZFilename);

	void renderShadow(Shader* shaderProgram) {};
	void render();
	void update(float dt);
private:
	Camera* m_camera;
	CubeMapTexture* m_pCubemap;
	Mesh *m_skyMesh;
	float4x4* modelMatrix;
};


#endif // !__SKYBOXRENDERER_H__
