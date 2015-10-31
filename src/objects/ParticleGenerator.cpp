#include "ParticleGenerator.h"

#ifdef WIN32
#include <IL/il.h>
#include <IL/ilut.h>
#else
#include <il.h>
#include <ilut.h>
#endif // WIN32
#include "Logger.h"
#include "glutil/glutil.h"
#include "float3x3.h"
#include <algorithm>

using namespace chag;

float3 Particle::startPosition;


ParticleGenerator::ParticleGenerator(GLuint shaderProgram, GLuint texture, int amount, Camera *camera, float3 position)
	: m_shaderProgram(shaderProgram), m_texture(texture), m_amount(amount), m_camera(camera), m_position(position)
{
	
	GLfloat quad[] = { //POSITION3 TEXCOORD2
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 1.0f,

		0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,

	};

	glGenVertexArrays(1, &m_vaob);
	glBindVertexArray(m_vaob);

	GLuint pos_vbo;
	glGenBuffers(1, &pos_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, pos_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	
	/* CLEANUP */
	glBindVertexArray(0);

	Particle::startPosition = this->m_position;
	for (int i = 0; i < amount; i++) {
		Particle *part = new Particle();
		part->init();
		this->m_particles.push_back(part);
	}
}


ParticleGenerator::~ParticleGenerator()
{
}


void ParticleGenerator::render() {
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glBindTexture(GL_TEXTURE_2D, m_texture);
	GLint current_program = 0;
	glGetIntegerv(GL_CURRENT_PROGRAM, &current_program);
	glUseProgram(m_shaderProgram);

	setUniformSlow(m_shaderProgram, "color", make_vector(1.0f, 1.0f, 1.0f));
	glUniform1i(glGetUniformLocation(current_program, "sprite"), 0);

	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);

	float3x3 modelMatrix3x3 = getModelMatrix3x3();
	float4x4 projectionMatrix = m_camera->getProjectionMatrix();
	float4x4 viewMatrix = m_camera->getViewMatrix();
	float4x4 vpMatrix = projectionMatrix * viewMatrix;

	glBindVertexArray(m_vaob);
	
	std::vector<Particle*> particles = this->m_particles;
	std::sort(particles.begin(), particles.end(), [this](Particle* p1, Particle* p2) { return length(this->m_camera->getPosition() - p1->position) > length(this->m_camera->getPosition() - p2->position); });
	for (Particle *particle : particles) {
		if (particle->life > 0.0f) {
			float4x4 modelMatrix4x4 = make_matrix(modelMatrix3x3, particle->position) * make_scale<float4x4>(make_vector(0.1f, 0.1f, 0.1f));
			float4x4 mvpMatrix = vpMatrix * modelMatrix4x4;
			
			glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, "mvpMatrix"), 1, false, &mvpMatrix.c1.x);

			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
	}

	/* CLEANUP */
	glBindVertexArray(0);
	glUseProgram(current_program);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_BLEND);

	glEnable(GL_CULL_FACE);
}

void ParticleGenerator::update(float dt) {
	for (Particle *particle : this->m_particles) {
		if (particle->life > 0.0f){
			particle->position += particle->velocity;
			particle->life -= dt;
		}
		else {
			particle->init();
		}
	}
}

float3x3 ParticleGenerator::getModelMatrix3x3() {
	float3 u = normalize(m_camera->getUp());
	float3 n = normalize(-(m_camera->getPosition() - m_camera->getLookAt()));
	float3 r = normalize(cross(u, n));

	float3 uprim = cross(n, r);
	
	return make_matrix(r,uprim,n);
}
