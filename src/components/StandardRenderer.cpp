#include "StandardRenderer.h"

#define NORMAL_TEXTURE_LOCATION 3
#define DIFFUSE_TEXTURE_LOCATION 0

StandardRenderer::StandardRenderer(){

}

StandardRenderer::StandardRenderer(Mesh* mesh, float4x4* modelMatrix, Shader* shaderProgram):
        mesh(mesh), modelMatrix(modelMatrix), shaderProgram(shaderProgram)
{

}


void StandardRenderer::update(float dt){
    ;
}

void StandardRenderer::render() {
    CHECK_GL_ERROR();

    chag::float4x4 normalMatrix = chag::inverse(chag::transpose(*modelMatrix));
    shaderProgram->setUniformMatrix4fv("modelMatrix", *modelMatrix);
    shaderProgram->setUniformMatrix4fv("normalMatrix", normalMatrix);

    for (size_t i = 0; i < mesh->m_chunks.size(); i++) {
        CHECK_GL_ERROR();

        Chunk &chunk = mesh->m_chunks[i];

        if (mesh->materials[chunk.materialIndex].diffuseTexture != NULL) {
            shaderProgram->setUniform1i("diffuse_texture", DIFFUSE_TEXTURE_LOCATION);
            mesh->materials[chunk.materialIndex].diffuseTexture->bind(GL_TEXTURE0);
        }
        if (mesh->materials[chunk.materialIndex].bumpMapTexture != NULL) {
            shaderProgram->setUniform1i("normal_texture", NORMAL_TEXTURE_LOCATION);
            mesh->materials[chunk.materialIndex].bumpMapTexture->bind(GL_TEXTURE3);
        }

        shaderProgram->setUniform1i("has_diffuse_texture", mesh->materials[chunk.materialIndex].diffuseTexture != NULL);
        shaderProgram->setUniform3f("material_diffuse_color", mesh->materials[chunk.materialIndex].diffuseColor);
        shaderProgram->setUniform3f("material_specular_color", mesh->materials[chunk.materialIndex].specularColor);
        shaderProgram->setUniform3f("material_ambient_color", mesh->materials[chunk.materialIndex].ambientColor);
        shaderProgram->setUniform3f("material_emissive_color", mesh->materials[chunk.materialIndex].emissiveColor);
        shaderProgram->setUniform1i("has_normal_texture", mesh->materials[chunk.materialIndex].bumpMapTexture != NULL);
        shaderProgram->setUniform1f("material_shininess", mesh->materials[chunk.materialIndex].specularExponent);
        CHECK_GL_ERROR();

        glBindVertexArray(chunk.m_vaob);
        glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, chunk.m_ind_bo);

        glDrawElements(GL_TRIANGLES, mesh->m_chunks[i].m_numIndices, GL_UNSIGNED_INT, 0);
        CHECK_GL_ERROR();
    }
    CHECK_GL_ERROR();
}

void StandardRenderer::renderShadow(Shader *shaderProgram) {

    shaderProgram->setUniformMatrix4fv("modelMatrix", *modelMatrix);

    for (size_t i = 0; i < mesh->m_chunks.size(); i++) {
        CHECK_GL_ERROR();

        Chunk &chunk = mesh->m_chunks[i];

        glBindVertexArray(chunk.m_vaob);
        glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, chunk.m_ind_bo);

        glDrawElements(GL_TRIANGLES, mesh->m_chunks[i].m_numIndices, GL_UNSIGNED_INT, 0);
        CHECK_GL_ERROR();
    }

    CHECK_GL_ERROR();
}