
#include "Mesh.h"

#include "Logger.h"
#include <IL/il.h>
#include <IL/ilut.h>
#include <glutil\glutil.h>

using namespace chag;

Mesh::Mesh() {};

Mesh::~Mesh() {};

void Mesh::render() {
	CHECK_GL_ERROR();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	for (size_t i = 0; i < m_chunks.size(); ++i)
	{
		CHECK_GL_ERROR();
		Chunk &chunk = m_chunks[i];
		if (m_textures[chunk.m_textureIndex].diffuse_map_id != -1) {
			glActiveTexture(GL_TEXTURE0);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, m_textures[chunk.m_textureIndex].diffuse_map_id);
		}

		GLint current_program = 0;
		glGetIntegerv(GL_CURRENT_PROGRAM, &current_program);
		glUniform1i(glGetUniformLocation(current_program, "has_diffuse_texture"), m_textures[chunk.m_textureIndex].diffuse_map_id != -1);
		glUniform3fv(glGetUniformLocation(current_program, "material_diffuse_color"), 1, &m_textures[chunk.m_textureIndex].diffuseColor.x);
		glUniform3fv(glGetUniformLocation(current_program, "material_specular_color"), 1, &m_textures[chunk.m_textureIndex].specularColor.x);
		glUniform3fv(glGetUniformLocation(current_program, "material_ambient_color"), 1, &m_textures[chunk.m_textureIndex].ambientColor.x);
		glUniform3fv(glGetUniformLocation(current_program, "material_emissive_color"), 1, &m_textures[chunk.m_textureIndex].emissiveColor.x);
		glUniform1f(glGetUniformLocation(current_program, "material_shininess"), m_textures[chunk.m_textureIndex].specularExponent);
		CHECK_GL_ERROR();

		glBindVertexArray(chunk.m_vaob);
		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, chunk.m_ind_bo);
		//glDrawArrays(GL_TRIANGLES, 0, (GLsizei)chunk.m_positions.size());

		glDrawElements(GL_TRIANGLES, m_chunks[i].m_numIndices, GL_UNSIGNED_INT, 0);
		CHECK_GL_ERROR();
	}

	glPopAttrib();
	CHECK_GL_ERROR();
}

bool Mesh::loadMesh(const std::string& fileName) {
	Assimp::Importer importer;

	const aiScene* pScene = importer.ReadFile(
		fileName.c_str(), aiProcess_GenSmoothNormals | aiProcess_Triangulate );
	

	if (!pScene) {
		Logger l = Logger::instance();
		l.logDebug("Error loading mesh for " + fileName);
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

		m.diffuse_map_id = getTexture(material, fileName, aiTextureType_DIFFUSE);
		m.bump_map_id = getTexture(material, fileName, aiTextureType_HEIGHT);
		



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

		m.ambientColor = make_vector(ambient.r, ambient.g, ambient.b, 1.0f);
		m.diffuseColor = make_vector(diffuse.r, diffuse.g, diffuse.b, 1.0f);
		m.specularColor = make_vector(specular.r, specular.g, specular.b, 1.0f);
		m.emissiveColor = make_vector(emissive.r, emissive.g, emissive.b, 1.0f);
		m.specularExponent = specExp > 0.0f ? specExp : 0.0f;

		m_textures.push_back(m);
	}
}

GLuint Mesh::getTexture(const aiMaterial *material, const std::string& fileName, aiTextureType type) {
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

	if (material->GetTextureCount(type) > 0) {
			aiString path;

			if (material->GetTexture(type, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
				string p(path.data);

				if (p.substr(0, 2) == ".\\") {
					p = p.substr(2, p.size() - 2);
				}
				string fullPath = dir + "/" + p;

				Logger l = Logger::instance();
				l.logInfo("Loading texture: " + fullPath);
				return loadTexture(fullPath);
			}
			else
			{
				return -1;
			}
		}
		else
		{
			return -1;
		}
}

void Mesh::initMesh(unsigned int index, const aiMesh* paiMesh) {
	std::vector<float3> positions;
	std::vector<float3> normals;
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

	Chunk c(positions, normals, uvs, indices, paiMesh->mMaterialIndex);
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



	m_chunks.push_back(c);
}


Mesh::Chunk::Chunk(std::vector<chag::float3>& positions,
	std::vector<chag::float3>& normals,
	std::vector<chag::float2>& uvs,
	std::vector<unsigned int>& indices,
	unsigned int textureIndex) :
	m_positions(positions), m_normals(normals), m_uvs(uvs), m_indices(indices), m_textureIndex(textureIndex)
{
	m_numIndices = indices.size();
}


GLuint Mesh::loadTexture(std::string fileName)
{
	ILuint image = ilGenImage();
	ilBindImage(image);
	CHECK_GL_ERROR();

	if (ilLoadImage(fileName.c_str()) == IL_FALSE)
	{
		Logger l = Logger::instance();
		l.logSevere("Error to load texture " + fileName);
		ILenum Error;
		while ((Error = ilGetError()) != IL_NO_ERROR)
		{
			printf("  %d: %s\n", Error, iluErrorString(Error));
		}
		ilDeleteImage(image);
		return 0;
	}
	CHECK_GL_ERROR();
	// why not?
	if (ilTypeFromExt(fileName.c_str()) == IL_PNG || ilTypeFromExt(fileName.c_str()) == IL_JPG)
	{
		iluFlipImage();
	}
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	GLuint texid;
	glGenTextures(1, &texid);
	glActiveTexture(GL_TEXTURE0);
	CHECK_GL_ERROR();
	glBindTexture(GL_TEXTURE_2D, texid);
	CHECK_GL_ERROR();
	int width = ilGetInteger(IL_IMAGE_WIDTH);
	int height = ilGetInteger(IL_IMAGE_HEIGHT);
	// Note: now with SRGB 
	ILubyte* b = ilGetData();
	ILubyte c = *b;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, b);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());
	CHECK_GL_ERROR();
	ilDeleteImage(image);
	CHECK_GL_ERROR();
	//GLuint texid = ilutGLLoadImage(const_cast<char *>(fileName.c_str()));
	glGenerateMipmap(GL_TEXTURE_2D);
	CHECK_GL_ERROR();
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
	CHECK_GL_ERROR();

	glBindTexture(GL_TEXTURE_2D, 0);
	CHECK_GL_ERROR();
	return texid;
}

