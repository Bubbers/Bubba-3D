#ifndef __PARTICLE_GEN__
#define __PARTICLE_GEN__

#include "float3.h"
#include <vector>
#include "IDrawable.h"
#include "GL/glew.h"
#include "Camera.h"

using namespace chag;



struct Particle {
	float3 position;
	float3 velocity;
	float3 color;
	float  life;

	static float3 startPosition;

	Particle() : position(make_vector(0.0f, 0.0f, 0.0f)), velocity(make_vector(0.0f, 0.1f, 0.0f)), color(make_vector(0.0f, 0.0f, 0.0f)), life(0.0f) {	};

	void init() {
		position = startPosition;
		velocity = make_vector((((rand() % 1000) - 500.0f) / 50000.0f), (((rand() % 1000) - 500.0f) / 50000.0f), (((rand() % 1000) - 500.0f) / 50000.0f));
		life = 2000.0f + (rand() % 2000);
	}
};

class ParticleGenerator : public IDrawable
{
public:
	ParticleGenerator(GLuint shaderprogram, GLuint texture, int amount, Camera *camera, float3 position);
	~ParticleGenerator();

	void update(float dt);

	void render();

	Camera *m_camera;

private:
	std::vector<Particle*> m_particles;
	GLuint m_vaob;
	GLuint m_texture;
	GLuint m_shaderProgram;
	int m_amount;
	float3 m_position;

	float3x3 getModelMatrix3x3();
};

#endif
