#include <Logger.h>
#include "GameObject.h"
#include "float3x3.h"
#include "ResourceManager.h"

#define SIMPLE_SHADER_NAME "simple_shader"
#define NORMAL_TEXTURE_LOCATION 3
#define DIFFUSE_TEXTURE_LOCATION 0

GameObject::GameObject(Mesh mesh) {
    this->mesh = mesh;
    this->shininess = 0.0f;
    this->shaderProgram = ResourceManager::getShader(SIMPLE_SHADER_NAME);
};

void GameObject::render() {
    CHECK_GL_ERROR();
    glPushAttrib(GL_ALL_ATTRIB_BITS);

    chag::float4x4 normalMatrix = chag::inverse(chag::transpose(mesh.m_modelMatrix));
    shaderProgram.setUniformMatrix4fv("modelMatrix", mesh.m_modelMatrix);
    shaderProgram.setUniformMatrix4fv("normalMatrix", normalMatrix);

    for (size_t i = 0; i < mesh.m_chunks.size(); i++) {
        CHECK_GL_ERROR();

        Chunk &chunk = mesh.m_chunks[i];

        if (mesh.materials[chunk.materialIndex].diffuseTexture != NULL) {
            shaderProgram.setUniform1i("diffuse_texture", DIFFUSE_TEXTURE_LOCATION);
            mesh.materials[chunk.materialIndex].diffuseTexture->bind(GL_TEXTURE0);
        }
        if (mesh.materials[chunk.materialIndex].bumpMapTexture != NULL) {
            shaderProgram.setUniform1i("normal_texture", NORMAL_TEXTURE_LOCATION);
            mesh.materials[chunk.materialIndex].bumpMapTexture->bind(GL_TEXTURE3);
        }

        shaderProgram.setUniform1i("has_diffuse_texture", mesh.materials[chunk.materialIndex].diffuseTexture != NULL);
        shaderProgram.setUniform3f("material_diffuse_color", mesh.materials[chunk.materialIndex].diffuseColor);
        shaderProgram.setUniform3f("material_specular_color", mesh.materials[chunk.materialIndex].specularColor);
        shaderProgram.setUniform3f("material_ambient_color", mesh.materials[chunk.materialIndex].ambientColor);
        shaderProgram.setUniform3f("material_emissive_color", mesh.materials[chunk.materialIndex].emissiveColor);
        shaderProgram.setUniform1i("has_normal_texture", mesh.materials[chunk.materialIndex].bumpMapTexture != NULL);
        shaderProgram.setUniform1f("material_shininess", mesh.materials[chunk.materialIndex].specularExponent);
        CHECK_GL_ERROR();

        glBindVertexArray(chunk.m_vaob);
        glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, chunk.m_ind_bo);

        glDrawElements(GL_TRIANGLES, mesh.m_chunks[i].m_numIndices, GL_UNSIGNED_INT, 0);
        CHECK_GL_ERROR();
    }

    glPopAttrib();
    CHECK_GL_ERROR();
}


void GameObject::renderShadow(Shader shaderProgram) {
    glPushAttrib(GL_ALL_ATTRIB_BITS);

    shaderProgram.setUniformMatrix4fv("modelMatrix", mesh.m_modelMatrix);

    for (size_t i = 0; i < mesh.m_chunks.size(); i++) {
        CHECK_GL_ERROR();

        Chunk &chunk = mesh.m_chunks[i];

        glBindVertexArray(chunk.m_vaob);
        glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, chunk.m_ind_bo);

        glDrawElements(GL_TRIANGLES, mesh.m_chunks[i].m_numIndices, GL_UNSIGNED_INT, 0);
        CHECK_GL_ERROR();
    }

    glPopAttrib();
    CHECK_GL_ERROR();
}