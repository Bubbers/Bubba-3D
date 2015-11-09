#include <ResourceManager.h>
#include "Mesh.h"
#include <exception>
#include "Logger.h"
#include "ResourceManager.h"

using namespace chag;

Mesh::Mesh(){
    m_modelMatrix = make_identity<float4x4>();
};

Mesh::~Mesh() {};

Chunk::Chunk(std::vector<chag::float3>& positions,
	std::vector<chag::float3>& normals,
	std::vector<chag::float2>& uvs,
	std::vector<unsigned int>& indices,
	std::vector<chag::float3>& tangents,
	std::vector<chag::float3>& bittangents,
	unsigned int materialIndex) :
	m_positions(positions), m_normals(normals), m_uvs(uvs), m_indices(indices), materialIndex(materialIndex), m_tangents(tangents), m_bittangents(bittangents)
{
	m_numIndices = indices.size();
}

bool Mesh::loadMesh(const std::string& fileName) {
  Logger::logInfo("Loading mesh " + fileName);
  Assimp::Importer importer;

    const aiScene* pScene = importer.ReadFile(
      fileName.c_str(), aiProcess_GenSmoothNormals | aiProcess_Triangulate | aiProcess_CalcTangentSpace);

	if (!pScene) {
	  Logger::logSevere("Error loading mesh for " + fileName);
	}
	else {
		initFromScene(pScene, fileName);
	}
	
	return true;
}

bool Mesh::initFromScene(const aiScene* pScene, const std::string& fileName) {

	for (unsigned int i = 0; i < pScene->mNumMeshes; i++) {
		const aiMesh* paiMesh = pScene->mMeshes[i];
		initMesh(i, paiMesh);
	}

	initMats(pScene, fileName);
	
	return true;
}

void Mesh::initMats(const aiScene* pScene, const std::string& fileName) {
	std::string::size_type index = fileName.find_last_of("/");
	std::string dir;

	if (index == std::string::npos) {
		dir = ".";
	}
	else if (index == 0) {
		dir = "/";
	}
	else {
		dir = fileName.substr(0, index);
	}

	for (unsigned int i = 0; i < pScene->mNumMaterials; i++) {
		const aiMaterial* material = pScene->mMaterials[i];
		Material m;

		m.diffuseTexture = getTexture(material, fileName, aiTextureType_DIFFUSE);
		m.bumpMapTexture = getTexture(material, fileName, aiTextureType_HEIGHT);

		aiColor3D diffuse;
		aiColor3D ambient;
		aiColor3D specular;
		aiColor3D emissive;
		float specExp;

		material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
		material->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
		material->Get(AI_MATKEY_COLOR_SPECULAR, specular);
		material->Get(AI_MATKEY_COLOR_EMISSIVE, emissive);
		material->Get(AI_MATKEY_SHININESS, specExp);

		m.ambientColor = make_vector(ambient.r, ambient.g, ambient.b);
		m.diffuseColor = make_vector(diffuse.r, diffuse.g, diffuse.b);
		m.specularColor = make_vector(specular.r, specular.g, specular.b);
		m.emissiveColor = make_vector(emissive.r, emissive.g, emissive.b);
		m.specularExponent = specExp > 0.0f ? specExp : 0.0f;

		materials.push_back(m);
	}
}

Texture* Mesh::getTexture(const aiMaterial *material, const std::string& fileName, aiTextureType type) {
	std::string::size_type index = fileName.find_last_of("/");
	std::string dir;
    aiString path;

	if (index == std::string::npos) {
		dir = ".";
	}
	else if (index == 0) {
		dir = "/";
	}
	else {
		dir = fileName.substr(0, index);
	}

	if (material->GetTextureCount(type) <= 0) {
        return NULL;
    }

    if (material->GetTexture(type, 0, &path, NULL, NULL, NULL, NULL, NULL) != AI_SUCCESS) {
        return NULL;
    }
    string p(path.data);

    if (p.substr(0, 2) == ".\\") {
        p = p.substr(2, p.size() - 2);
    }
    string fullPath = dir + "/" + p;

    return ResourceManager::loadAndFetchTexture(fullPath);
}

void Mesh::initMesh(unsigned int index, const aiMesh* paiMesh) {
	std::vector<float3> positions;
	std::vector<float3> normals;
	std::vector<float3> tangents;
	std::vector<float3> bittangents;
	std::vector<float2> uvs;
	std::vector<unsigned int> indices;

	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);
	
	float3 minV = make_vector(FLT_MAX, FLT_MAX, FLT_MAX);
	float3 maxV = make_vector(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {
		const aiVector3D pPos = paiMesh->mVertices[i];
		const aiVector3D pNormal = paiMesh->mNormals[i];
		const aiVector3D pTexCoord = paiMesh->HasTextureCoords(0) ? paiMesh->mTextureCoords[0][i] : Zero3D;
		

		positions.push_back(make_vector(pPos.x, pPos.y, pPos.z));
		normals.push_back(make_vector(pNormal.x, pNormal.y, pNormal.z));
		uvs.push_back(make_vector(pTexCoord.x, pTexCoord.y));
		if (paiMesh->HasTangentsAndBitangents()) {
			const aiVector3D pBitTangents = paiMesh->mBitangents[i];
			const aiVector3D pTangents = paiMesh->mTangents[i];
			bittangents.push_back(make_vector(pBitTangents.x, pBitTangents.y, pBitTangents.z));
			tangents.push_back(make_vector(pTangents.x, pTangents.y, pTangents.z));
		}

		checkMinMax(pPos.x, pPos.y, pPos.z, &minV, &maxV);
	}
	
	checkMinMax(minV.x, minV.y, minV.z, &m_aabb.minV, &m_aabb.maxV);
	checkMinMax(maxV.x, maxV.y, maxV.z, &m_aabb.minV, &m_aabb.maxV);

	for (unsigned int i = 0; i < paiMesh->mNumFaces; i++) {
		const aiFace face = paiMesh->mFaces[i];
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}



	Chunk c(positions, normals, uvs, indices, tangents, bittangents, paiMesh->mMaterialIndex);
	glGenVertexArrays(1, &c.m_vaob);
	glBindVertexArray(c.m_vaob);

	glGenBuffers(1, &c.m_positions_bo);
	glBindBuffer(GL_ARRAY_BUFFER_ARB, c.m_positions_bo);
	glBufferData(GL_ARRAY_BUFFER_ARB, c.m_positions.size() * sizeof(float3),&c.m_positions[0].x, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &c.m_normals_bo);
	glBindBuffer(GL_ARRAY_BUFFER_ARB, c.m_normals_bo);
	glBufferData(GL_ARRAY_BUFFER_ARB, c.m_normals.size() * sizeof(float3),
		&c.m_normals[0].x, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(1);

	if (c.m_uvs.size() > 0){
		glGenBuffers(1, &c.m_uvs_bo);
		glBindBuffer(GL_ARRAY_BUFFER_ARB, c.m_uvs_bo);
		glBufferData(GL_ARRAY_BUFFER_ARB, c.m_uvs.size() * sizeof(float2),
			&c.m_uvs[0].x, GL_STATIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, false, 0, 0);
	}
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &c.m_ind_bo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, c.m_ind_bo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER_ARB, c.m_numIndices* sizeof(unsigned int), &c.m_indices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(3, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(3);

	if (c.m_bittangents.size() > 0) {
		glGenBuffers(1, &c.m_tangents_bo);
		glBindBuffer(GL_ARRAY_BUFFER_ARB, c.m_tangents_bo);
		glBufferData(GL_ARRAY_BUFFER_ARB, c.m_tangents.size() * sizeof(float3), &c.m_tangents[0].x, GL_STATIC_DRAW);
		glVertexAttribPointer(4, 3, GL_FLOAT, false, 0, 0);
		glEnableVertexAttribArray(4);

		glGenBuffers(1, &c.m_bittangents_bo);
		glBindBuffer(GL_ARRAY_BUFFER_ARB, c.m_bittangents_bo);
		glBufferData(GL_ARRAY_BUFFER_ARB, c.m_bittangents.size() * sizeof(float3), &c.m_bittangents[0].x, GL_STATIC_DRAW);
		glVertexAttribPointer(5, 3, GL_FLOAT, false, 0, 0);
		glEnableVertexAttribArray(5);

	}


	m_chunks.push_back(c);
}