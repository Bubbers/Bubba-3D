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
	m_skyObject->mesh.m_modelMatrix = modelMat;
	setUniformSlow(m_shaderProgram, "viewProjectionMatrix", viewProj);
	m_pCubemap->bind(GL_TEXTURE0);
	setUniformSlow(m_shaderProgram, "cubeMapSampler", 0);

	setUniformSlow(m_shaderProgram, "modelMatrix", m_skyMesh.m_modelMatrix);

	GLint current_program = 0;
	glGetIntegerv(GL_CURRENT_PROGRAM, &current_program);
	Mesh mesh = m_skyMesh;

	CHECK_GL_ERROR();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	printf("current %d\n", current_program);
	printf("and     %d\n", this->shaderProgram.shaderID);

	chag::float4x4 normalMatrix = chag::inverse(chag::transpose(mesh.m_modelMatrix));
	setUniformSlow(current_program, "modelMatrix", mesh.m_modelMatrix);
	setUniformSlow(current_program, "normalMatrix", normalMatrix);

	for (size_t i = 0; i < mesh.m_chunks.size(); ++i)
	{
		CHECK_GL_ERROR();

		Chunk &chunk = mesh.m_chunks[i];

		if (mesh.m_textures[chunk.m_textureIndex].diffuse_map_id != -1) {
			glActiveTexture(GL_TEXTURE0);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, mesh.m_textures[chunk.m_textureIndex].diffuse_map_id);
		}
		if (mesh.m_textures[chunk.m_textureIndex].bump_map_id != -1) {
			glUniform1i(glGetUniformLocation(current_program, "normal_texture"), 3);
			glActiveTexture(GL_TEXTURE3);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, mesh.m_textures[chunk.m_textureIndex].bump_map_id);
		}


		glUniform1i(glGetUniformLocation(current_program, "has_diffuse_texture"), mesh.m_textures[chunk.m_textureIndex].diffuse_map_id != -1);
		glUniform3fv(glGetUniformLocation(current_program, "material_diffuse_color"), 1, &mesh.m_textures[chunk.m_textureIndex].diffuseColor.x);
		glUniform3fv(glGetUniformLocation(current_program, "material_specular_color"), 1, &mesh.m_textures[chunk.m_textureIndex].specularColor.x);
		glUniform3fv(glGetUniformLocation(current_program, "material_ambient_color"), 1, &mesh.m_textures[chunk.m_textureIndex].ambientColor.x);
		glUniform3fv(glGetUniformLocation(current_program, "material_emissive_color"), 1, &mesh.m_textures[chunk.m_textureIndex].emissiveColor.x);
		glUniform1i(glGetUniformLocation(current_program, "has_normal_texture"), mesh.m_textures[chunk.m_textureIndex].bump_map_id != -1);
		glUniform1f(glGetUniformLocation(current_program, "material_shininess"), mesh.m_textures[chunk.m_textureIndex].specularExponent);
		CHECK_GL_ERROR();

		glBindVertexArray(chunk.m_vaob);
		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, chunk.m_ind_bo);
		//glDrawArrays(GL_TRIANGLES, 0, (GLsizei)chunk.m_positions.size());

		glDrawElements(GL_TRIANGLES, mesh.m_chunks[i].m_numIndices, GL_UNSIGNED_INT, 0);
		CHECK_GL_ERROR();
	}
	m_skyObject->render();

	glPopAttrib();
	CHECK_GL_ERROR();

	glCullFace(OldCullFaceMode);
	glDepthFunc(OldDepthFuncMode);
	glUseProgram(currentProgram);
}



Skybox::~Skybox()
{
}
