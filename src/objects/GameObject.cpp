#include <Logger.h>
#include "GameObject.h"
#include "float3x3.h"
#include "ResourceManager.h"
#include "constants.h"

#define NORMAL_TEXTURE_LOCATION 3
#define DIFFUSE_TEXTURE_LOCATION 0


GameObject::GameObject() {
    m_modelMatrix = make_identity<float4x4>();
}

GameObject::GameObject(Mesh *mesh) {
    GameObject();
    this->mesh = mesh;
    shininess = 0.0f;
    shaderProgram = ResourceManager::getShader(SIMPLE_SHADER_NAME);
    shaderProgram->setUniformBufferObjectBinding(UNIFORM_BUFFER_OBJECT_MATRICES_NAME, UNIFORM_BUFFER_OBJECT_MATRICES_INDEX);
};

GameObject::~GameObject() {
    mesh = nullptr;
    shaderProgram = nullptr;
}

void GameObject::move(float4x4 model_matrix) {
    m_modelMatrix = model_matrix;
}

void GameObject::update(float4x4 update_matrix) {
    m_modelMatrix = m_modelMatrix * update_matrix;
}

void GameObject::render() {
    CHECK_GL_ERROR();

    chag::float4x4 normalMatrix = chag::inverse(chag::transpose(m_modelMatrix));
    shaderProgram->setUniformMatrix4fv("modelMatrix", m_modelMatrix);
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

std::vector<Triangle *> GameObject::getTriangles() {
    std::vector<Triangle *> ts;

    for (int i = 0; i < mesh->m_chunks.size(); i++) {

        for (int j = 0; j < mesh->m_chunks[i].m_positions.size(); j += 3) {

            float4 p1 = m_modelMatrix * make_vector(mesh->m_chunks[i].m_positions[j + 0].x,
                                                    mesh->m_chunks[i].m_positions[j + 0].y,
                                                    mesh->m_chunks[i].m_positions[j + 0].z, 1.0f);

            float4 p2 = m_modelMatrix * make_vector(mesh->m_chunks[i].m_positions[j + 1].x,
                                                    mesh->m_chunks[i].m_positions[j + 1].y,
                                                    mesh->m_chunks[i].m_positions[j + 1].z, 1.0f);

            float4 p3 = m_modelMatrix * make_vector(mesh->m_chunks[i].m_positions[j + 2].x,
                                                    mesh->m_chunks[i].m_positions[j + 2].y,
                                                    mesh->m_chunks[i].m_positions[j + 2].z, 1.0f);

            Triangle *t = new Triangle(make_vector(p1.x, p1.y, p1.z), make_vector(p2.x, p2.y, p2.z),
                                       make_vector(p3.x, p3.y, p3.z));

            ts.push_back(t);
        }
    }

    return ts;
}


void GameObject::renderShadow(Shader *shaderProgram) {

    shaderProgram->setUniformMatrix4fv("modelMatrix", m_modelMatrix);

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