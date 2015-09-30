#include "GameObject.h"

GameObject::GameObject(Mesh mesh) {
  this->mesh = mesh;
  this->shininess = 0.0f;
};

void GameObject::render() {
  CHECK_GL_ERROR();
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  GLint current_program = 0;
  glGetIntegerv(GL_CURRENT_PROGRAM, &current_program);
  setUniformSlow(current_program, "modelMatrix", mesh.m_modelMatrix);

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

  glPopAttrib();
  CHECK_GL_ERROR();
}


