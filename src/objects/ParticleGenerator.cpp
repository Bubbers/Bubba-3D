#include "ParticleGenerator.h"

#include "float3x3.h"

using namespace chag;

float3* Particle::startPosition = NULL; 

ParticleGenerator::ParticleGenerator(Shader shaderProgram, Texture texture, int amount, Camera *camera, float3 position)
	: shaderProgram(shaderProgram), texture(texture), m_amount(amount), m_camera(camera), m_position(position)
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
		Particle *part = new Particle(&this->m_position);
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

	shaderProgram.backupCurrentShaderProgram();
	shaderProgram.use();

	texture.bind(GL_TEXTURE0);

	shaderProgram.setUniform3f("color", make_vector(1.0f, 1.0f, 1.0f));
	shaderProgram.setUniform1i("sprite", 0);

	float3x3 modelMatrix3x3 = getModelMatrix3x3();
	float4x4 projectionMatrix = m_camera->getProjectionMatrix();
	float4x4 viewMatrix = m_camera->getViewMatrix();
	float4x4 vpMatrix = projectionMatrix * viewMatrix;

	float distance = length(this->m_camera->getPosition() - this->m_position);
	int maxParticles = (m_amount * LOD_FACTOR / distance );
	glBindVertexArray(m_vaob);
	
	std::vector<Particle*> particles = this->m_particles;
	std::sort(particles.begin(), particles.end(), [this](Particle* p1, Particle* p2) { return length(this->m_camera->getPosition() - p1->position) > length(this->m_camera->getPosition() - p2->position); });

	int iterations = 0;
	for (Particle *particle : particles) {
		if (iterations > maxParticles) { break; }
		iterations++;

		if (particle->life > 0.0f) {
			float3 scale = make_vector(0.1f, 0.1f, 0.1f) * (1.0 + distance / LINEAR_SCALE_FACTOR);
			float4x4 modelMatrix4x4 = make_matrix(modelMatrix3x3, particle->position) * make_scale<float4x4>(scale);
			float4x4 mvpMatrix = vpMatrix * modelMatrix4x4;
			
			shaderProgram.setUniformMatrix4fv("mvpMatrix", mvpMatrix);

			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
	}

	/* CLEANUP */
	glBindVertexArray(0);
	shaderProgram.restorePreviousShaderProgram();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_BLEND);

	glEnable(GL_CULL_FACE);
}

void ParticleGenerator::update(float dt) {
	float distance = length(this->m_camera->getPosition() - this->m_position);

	for (Particle *particle : this->m_particles) {
		if (particle->life > 0.0f){
			particle->position += particle->velocity * dt / 1000;
			particle->life -= dt + (distance * 2);

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
