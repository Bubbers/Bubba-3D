#ifndef __MESH_H__
#define __MESH_H__



#include <string>
#include <vector>
#include <float2.h>
#include <float3.h>
#include <float4.h>
#include <glutil/glutil.h>
#include "GL/glew.h"
#include "GL/glut.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "AABB2.h"

struct Material
{
  chag::float4 diffuseColor;
  chag::float4 ambientColor;
  chag::float4 specularColor;
  chag::float4 emissiveColor;
  float specularExponent;
  GLint diffuse_map_id; 
  GLint bump_map_id;
};

struct Chunk
{
  Chunk(std::vector<chag::float3>& positions,
	std::vector<chag::float3>& normals,
	std::vector<chag::float2>& uvs,
	std::vector<unsigned int>& indices,
	std::vector<chag::float3>& tangents,
	std::vector<chag::float3>& bittangents,
	unsigned int textureIndex);
  ~Chunk() {};

		
  // Data on host
  std::vector<chag::float3> m_positions;
  std::vector<chag::float3> m_normals;
  std::vector<chag::float2> m_uvs;
  std::vector<unsigned int> m_indices;
  std::vector<chag::float3> m_tangents;
  std::vector<chag::float3> m_bittangents;

  // Data on GPU
  GLuint	m_positions_bo;
  GLuint	m_normals_bo;
  GLuint	m_uvs_bo;
  GLuint  m_ind_bo;
  GLuint  m_tangents_bo;
  GLuint  m_bittangents_bo;
  // Vertex Array Object
  GLuint	m_vaob;

  unsigned int m_textureIndex;
  unsigned int m_numIndices;
};




class Mesh
{
 public:
  Mesh();
  ~Mesh();

  bool loadMesh(const std::string& fileName);
  void render();
  GLuint getDiffuseTexture(int chunk){
    return m_textures[m_chunks[chunk].m_textureIndex].diffuse_map_id;
  }
 private:

  bool initFromScene(const aiScene* pScene, const std::string& fileName);
  void initMesh(unsigned int index, const aiMesh* paiMesh);
  void initMats(const aiScene* pScene, const std::string& fileName);
  GLuint getTexture(const aiMaterial *material, const std::string& fileName, aiTextureType type);
  GLuint loadTexture(std::string fileName);

	
	
  std::vector<Material> m_textures;

 public: 

  chag::float4x4 m_modelMatrix;
  std::vector<Chunk> m_chunks;
  float shininess;
  AABB m_aabb;
};

#endif // !__MESH_H__
