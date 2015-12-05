#ifndef __PARTICLE_GEN__
#define __PARTICLE_GEN__

#include "float3.h"
#include <vector>
#include "IDrawable.h"
#include "GL/glew.h"
#include "Camera.h"
#include "Shader.h"
#include "Texture.h"
#include "ParticleConf.h"
#include "Particle.h"
#include "IRenderComponent.h"

#define LINEAR_SCALE_FACTOR 50.0f
#define LOD_FACTOR 25.0f


using namespace chag;


class ParticleGenerator : public IRenderComponent
{
public:
	ParticleGenerator(Texture *texture, int amount, Camera *camera, float3 position, ParticleConf *conf);
	~ParticleGenerator();

	void update(float dt);
	void renderShadow(Shader* shaderProgram) {};
	void render();

	Camera *m_camera;
	float3 m_position;

private:
	std::vector<Particle*> m_particles;
	GLuint m_vaob;
	Texture *texture;
	int m_amount;

	ParticleConf *conf;

	float3x3 getModelMatrix3x3();
};

#endif
