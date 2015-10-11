#include "ParticleGenerator.h"

#ifdef WIN32
#include <IL/il.h>
#include <IL/ilut.h>
#else
#include <il.h>
#include <ilut.h>
#endif // WIN32
#include "Logger.h"
#include "glutil\glutil.h"


ParticleGenerator::ParticleGenerator(GLuint shaderProgram, GLuint texture, int amount, Camera *camera)
	: m_shaderProgram(shaderProgram), m_texture(texture), m_amount(amount), m_camera(camera)
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

	for (int i = 0; i < amount; i++) {
		Particle *part = new Particle();
		part->position = make_vector(0.0f, 15.0f + ((rand() % 5) / 500.0f), 0.0f);
		part->velocity = make_vector((((rand() % 10) - 5.0f) / 500.0f), (((rand() % 10) - 5.0f) / 500.0f), (((rand() % 10) - 5.0f) / 500.0f));
		part->life = 1.0f + ((rand() % 5) / 5.0f);
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

	
	setUniformSlow(m_shaderProgram, "projectionMatrix", m_camera->getProjectionMatrix());
	setUniformSlow(m_shaderProgram, "viewMatrix", m_camera->getViewMatrix());
	setUniformSlow(m_shaderProgram, "color", make_vector(1.0f, 1.0f, 1.0f));
	m_camera->getProjectionMatrix();
	glUniform1i(glGetUniformLocation(current_program, "sprite"), 0);
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glBindVertexArray(m_vaob);
	
	std::vector<Particle*> particles = this->m_particles;
	std::sort(particles.begin(), particles.end(), [this](Particle* p1, Particle* p2) { return length(this->m_camera->getPosition() - p1->position) > length(this->m_camera->getPosition() - p2->position); });
	for (Particle *particle : particles) {
		if (particle->life > 0.0f) {
			setUniformSlow(m_shaderProgram, "offset", particle->position);

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

void ParticleGenerator::update() {
	for (Particle *particle : this->m_particles) {
		if (particle->life > 0.0f){
			particle->position += particle->velocity;
			particle->life -= 0.01; //TODO dt
		}
		else {
			particle->position = make_vector(0.0f, 15.0f + ((rand() % 5) / 500.0f), 0.0f);
			particle->velocity = make_vector((((rand() % 10) - 5.0f) / 500.0f), (((rand() % 10) - 5.0f) / 500.0f), (((rand() % 10) - 5.0f) / 500.0f));
			particle->life = 1.0f + ((rand() % 5) / 5.0f);
		}
	}
}

