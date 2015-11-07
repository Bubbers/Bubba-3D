#include "Skybox.h"

#include <Quaternion.h>

Skybox::Skybox(Camera* camera) : m_camera(camera)
{
}

bool Skybox::init(const string& posXFilename, const string& negXFilename, const string& posYFilename, const string& negYFilename, const string& posZFilename, const string& negZFilename) {
	m_pCubemap = new CubeMapTexture(posXFilename, negXFilename, posYFilename, negYFilename, posZFilename, negZFilename);

	m_shaderProgram = loadShaderProgram("../shaders/skybox.vert", "../shaders/skybox.frag");
	glBindAttribLocation(m_shaderProgram, 0, "position");
	glBindFragDataLocation(m_shaderProgram, 0, "fragmentColor");
	linkShaderProgram(m_shaderProgram);

	m_skyMesh.loadMesh("../scenes/sphere.obj");
	m_skyObject = new GameObject(m_skyMesh);
	return true;
}

void Skybox::render() {

	GLint currentProgram;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
	glUseProgram(m_shaderProgram);

	GLint OldCullFaceMode;
	glGetIntegerv(GL_CULL_FACE_MODE, &OldCullFaceMode);
	GLint OldDepthFuncMode;
	glGetIntegerv(GL_DEPTH_FUNC, &OldDepthFuncMode);

	glCullFace(GL_FRONT);
	glDepthFunc(GL_LEQUAL);

	float4x4 viewProj = m_camera->getProjectionMatrix() * m_camera->getViewMatrix();
	float4x4 modelMat = make_translation(make_vector(0.0f, 2.0f, 0.0f)) * make_translation(m_camera->getPosition()) * make_scale<float4x4>(make_vector(20.0f, 20.0f, 20.0f));
	m_skyMesh.m_modelMatrix = modelMat;
	setUniformSlow(m_shaderProgram, "viewProjectionMatrix", viewProj);
	m_pCubemap->bind(GL_TEXTURE0);
	setUniformSlow(m_shaderProgram, "cubeMapSampler", 0);
	setUniformSlow(m_shaderProgram, "modelMatrix", m_skyMesh.m_modelMatrix);

	Mesh mesh = m_skyMesh;

	for (size_t i = 0; i < mesh.m_chunks.size(); ++i)
	{
		Chunk &chunk = mesh.m_chunks[i];

		glBindVertexArray(chunk.m_vaob);
		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, chunk.m_ind_bo);

		glDrawElements(GL_TRIANGLES, mesh.m_chunks[i].m_numIndices, GL_UNSIGNED_INT, 0);
		CHECK_GL_ERROR();
	}

	glCullFace(OldCullFaceMode);
	glDepthFunc(OldDepthFuncMode);
	glUseProgram(currentProgram);
}



Skybox::~Skybox()
{
}
