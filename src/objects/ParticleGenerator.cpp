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


ParticleGenerator::ParticleGenerator(GLuint shaderProgram, GLuint texture, int amount)
	: m_shaderProgram(shaderProgram), m_texture(texture), m_amount(amount)
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

	this->m_particles.push_back(Particle()); // TODO AMOUNT
}


ParticleGenerator::~ParticleGenerator()
{
}

void ParticleGenerator::render() {
	glDisable(GL_CULL_FACE);

	glBindTexture(GL_TEXTURE_2D, m_texture);
	GLint current_program = 0;
	glGetIntegerv(GL_CURRENT_PROGRAM, &current_program);
	glUseProgram(m_shaderProgram);

	setUniformSlow(m_shaderProgram, "offset", make_vector(-0.0f, 15.0f, 0.0f));
	setUniformSlow(m_shaderProgram, "projectionMatrix", m_camera->getProjectionMatrix());
	setUniformSlow(m_shaderProgram, "viewMatrix", m_camera->getViewMatrix());
	m_camera->getProjectionMatrix();
	glUniform1i(glGetUniformLocation(current_program, "sprite"), 0);
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);

	glBindVertexArray(m_vaob);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	/* CLEANUP */
	glBindVertexArray(0);
	glUseProgram(current_program);

	glEnable(GL_CULL_FACE);

}

