#include <Logger.h>
#include "GameObject.h"
#include "float3x3.h"
#include "ResourceManager.h"

#define SIMPLE_SHADER_NAME "simple_shader"


GameObject::GameObject(Mesh mesh) {
    this->mesh = mesh;
    this->shininess = 0.0f;
    this->shaderProgram = ResourceManager::getShader(SIMPLE_SHADER_NAME);
};

void GameObject::render() {
    CHECK_GL_ERROR();
    glPushAttrib(GL_ALL_ATTRIB_BITS);

	GLint current_program = 0;
	glGetIntegerv(GL_CURRENT_PROGRAM, &current_program);

    chag::float4x4 normalMatrix = chag::inverse(chag::transpose(mesh.m_modelMatrix));
    shaderProgram.setUniformMatrix4fv("modelMatrix", mesh.m_modelMatrix);
    shaderProgram.setUniformMatrix4fv("normalMatrix", normalMatrix);

    for (size_t i = 0; i < mesh.m_chunks.size(); ++i) {
        CHECK_GL_ERROR();

        Chunk &chunk = mesh.m_chunks[i];

        if (mesh.m_textures[chunk.m_textureIndex].diffuse_map_id != -1) {
            glActiveTexture(GL_TEXTURE0);
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, mesh.m_textures[chunk.m_textureIndex].diffuse_map_id);
        }
        if (mesh.m_textures[chunk.m_textureIndex].bump_map_id != -1) {
            shaderProgram.setUniform1i("normal_texture", 3);

            glActiveTexture(GL_TEXTURE3);
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, mesh.m_textures[chunk.m_textureIndex].bump_map_id);
        }

        shaderProgram.setUniform1i("has_diffuse_texture", mesh.m_textures[chunk.m_textureIndex].diffuse_map_id != -1);
        shaderProgram.setUniform3f("material_diffuse_color", mesh.m_textures[chunk.m_textureIndex].diffuseColor);
        shaderProgram.setUniform3f("material_specular_color", mesh.m_textures[chunk.m_textureIndex].specularColor);
        shaderProgram.setUniform3f("material_ambient_color", mesh.m_textures[chunk.m_textureIndex].ambientColor);
        shaderProgram.setUniform3f("material_emissive_color", mesh.m_textures[chunk.m_textureIndex].emissiveColor);
        shaderProgram.setUniform1i("has_normal_texture", mesh.m_textures[chunk.m_textureIndex].bump_map_id != -1);
        shaderProgram.setUniform1f("material_shininess", mesh.m_textures[chunk.m_textureIndex].specularExponent);
        CHECK_GL_ERROR();

        glBindVertexArray(chunk.m_vaob);
        glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, chunk.m_ind_bo);

        glDrawElements(GL_TRIANGLES, mesh.m_chunks[i].m_numIndices, GL_UNSIGNED_INT, 0);
        CHECK_GL_ERROR();
    }

    glPopAttrib();
    CHECK_GL_ERROR();
}


