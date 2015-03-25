#ifndef __OBJModel_h_
#define __OBJModel_h_

#include "GL/glew.h"
#include "GL/glut.h"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <float2.h>
#include <float3.h>
#include <float4.h>


class OBJModel
{
public:
	OBJModel(void);
	~OBJModel(void);
	/**
	* When called, renders the OBJModel
	*/
	void render();
	/**
	* Load the OBJModel from disk
	*/
	void load(std::string fileName); 
	GLuint getDiffuseTexture(int chunk){
		return m_chunks[chunk].material->diffuse_map_id; 
	}

  void setMaterialDiffuseTextureId(std::string matName, int textureId);

protected:

	size_t getNumVerts();

	void loadOBJ(std::ifstream &file, std::string basePath);
	void loadMaterials(std::string fileName, std::string basePath);
	unsigned int loadTexture(std::string fileName, std::string basePath);

	struct Material
	{
    chag::float4 diffuseColor;
		chag::float4 ambientColor;
		chag::float4 specularColor;
		chag::float4 emissiveColor;
		float specularExponent;
		int diffuse_map_id;
	};

	std::map<std::string, Material> m_materials;

public: 
	struct Chunk
	{
		Material *material;
		// Data on host
		std::vector<chag::float3> m_positions;
		std::vector<chag::float3> m_normals;
		std::vector<chag::float2> m_uvs; 
		// Data on GPU
		GLuint	m_positions_bo; 
		GLuint	m_normals_bo; 
		GLuint	m_uvs_bo; 
		// Vertex Array Object
		GLuint	m_vaob; 
	};
	std::vector<Chunk> m_chunks;
};

#endif // __OBJModel_h_
