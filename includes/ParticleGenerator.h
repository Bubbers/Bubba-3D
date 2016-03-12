#ifndef __PARTICLE_GEN__
#define __PARTICLE_GEN__

#include "linmath/float3.h"
#include <vector>
#include "IRenderComponent.h"

#define LINEAR_SCALE_FACTOR 50.0f
#define LOD_FACTOR 25.0f


using namespace chag;

class Camera;
class Particle;
class ParticleConf;

class ParticleGenerator : public IRenderComponent
{
public:
	ParticleGenerator(Texture *texture, int amount, Camera *camera, float3 position, ParticleConf *conf);
	~ParticleGenerator();

	void update(float dt);
	void renderShadow(ShaderProgram* shaderProgram) {};
	void render();

	void setScaleLod(bool value);

	Camera *m_camera;
	float3 m_position;

 	void setLooping(bool value);
private:
	std::vector<Particle*> m_particles;
	GLuint m_vaob;
	Texture *texture;
	int m_amount;

	ParticleConf *conf;
	bool doScale = true;

	float3x3 getModelMatrix3x3();
};

#endif
