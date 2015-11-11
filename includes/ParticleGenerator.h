#ifndef __PARTICLE_GEN__
#define __PARTICLE_GEN__

#include "float3.h"
#include <vector>
#include "IDrawable.h"
#include "GL/glew.h"
#include "Camera.h"
#include "Shader.h"
#include "Texture.h"


#define LINEAR_SCALE_FACTOR 50.0f
#define LOD_FACTOR 25.0f

#define PARTICLE_SPEED ((rand() % 6000) - 3000.0f) / 5000.0f
#define PARTICLE_LIFE 1000.0f + (rand() % 1000)

using namespace chag;

struct Particle {
	float3 position;
	float3 velocity;
	float3 color;
	float  life; //in ms

	static float3* startPosition;


	Particle(float3* pos) : position(make_vector(0.0f, 0.0f, 0.0f)), velocity(make_vector(0.0f, 0.1f, 0.0f)), color(make_vector(0.0f, 0.0f, 0.0f)), life(0.0f) {
          startPosition = pos;
	};

	void init() {
          position = *startPosition;
		velocity = make_vector(PARTICLE_SPEED, PARTICLE_SPEED, PARTICLE_SPEED);
		life     = PARTICLE_LIFE;
	}
};

class ParticleGenerator : public IDrawable
{
public:
	ParticleGenerator(Shader* shaderProgram, Texture *texture, int amount, Camera *camera, float3 position);
	~ParticleGenerator();

	void update(float dt);
	virtual void renderShadow(Shader* shaderProgram) {};

	void render();

	Camera *m_camera;
	float3 m_position;

private:
	std::vector<Particle*> m_particles;
	GLuint m_vaob;
	Texture *texture;
	int m_amount;
	Shader* shaderProgram;
	

	float3x3 getModelMatrix3x3();
};

#endif
