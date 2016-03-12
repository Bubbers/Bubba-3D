#include <ResourceManager.h>
#include "ParticleGenerator.h"
#include "constants.h"
#include "linmath/float3x3.h"
#include "Camera.h"
#include "Particle.h"
#include "ParticleConf.h"


using namespace chag;

ParticleGenerator::ParticleGenerator(Texture *texture, int amount, Camera *camera, float3 position, ParticleConf *conf)
	:  texture(texture), m_amount(amount), m_camera(camera), m_position(position), conf(conf)
{
	ResourceManager::loadShader("../shaders/particle.vert", "../shaders/particle.frag", "particleShader");
	shaderProgram = ResourceManager::getShader("particleShader");
	shaderProgram->setUniformBufferObjectBinding(UNIFORM_BUFFER_OBJECT_MATRICES_NAME,UNIFORM_BUFFER_OBJECT_MATRICES_INDEX);
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
		Particle *part = new Particle(&this->m_position, conf);
		this->m_particles.push_back(part);
	}
}


ParticleGenerator::~ParticleGenerator()
{

}

void ParticleGenerator::setScaleLod(bool value) {
	doScale = value;
}



void ParticleGenerator::render() {
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, conf->blendFunc);

	shaderProgram->backupCurrentShaderProgram();
	shaderProgram->use();

	texture->bind(GL_TEXTURE0);

	shaderProgram->setUniform3f("color", make_vector(1.0f, 1.0f, 1.0f));
	shaderProgram->setUniform1i("sprite", 0);

	float3x3 modelMatrix3x3 = getModelMatrix3x3();


	float distance = length(this->m_camera->getPosition() - this->m_position);
	int maxParticles = (int)(m_amount * LOD_FACTOR / distance );
	glBindVertexArray(m_vaob);
	
	std::vector<Particle*> particles = this->m_particles;
	std::sort(particles.begin(), particles.end(), [this](Particle* p1, Particle* p2) {
		float l1 = length(this->m_camera->getPosition() - p1->getPosition());
		float l2 = length(this->m_camera->getPosition() - p2->getPosition());

		return l1 > l2;
	});

	int iterations = 0;
	for (Particle *particle : particles) {
		if (iterations > maxParticles) { break; }
		iterations++;

		float3 scale;
		if (particle->isAlive()) {
			if(doScale) {
				scale = conf->getScale() * (1.0 + distance / LINEAR_SCALE_FACTOR);
			} else {
				scale = make_vector(1.0f, 1.0f, 1.0f);
			}
			float4x4 modelMatrix4x4 = make_matrix(modelMatrix3x3, particle->getPosition()) * make_scale<float4x4>(scale);

			shaderProgram->setUniformMatrix4fv("modelMatrix", modelMatrix4x4);

			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
	}

	/* CLEANUP */
	glBindVertexArray(0);
	shaderProgram->restorePreviousShaderProgram();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_BLEND);

	glEnable(GL_CULL_FACE);
}

void ParticleGenerator::update(float dt) {
	float distance = length(this->m_camera->getPosition() - this->m_position);

	for (Particle *particle : this->m_particles) {
		if (particle->isAlive()){
			particle->update(dt, distance, conf);
		}
		else if(conf->loop(dt)){
			particle->reset(conf);
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


void ParticleGenerator::setLooping(bool value) {
	conf->setLooping(value);
}