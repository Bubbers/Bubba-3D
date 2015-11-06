#include "CubeMapTexture.h"
#include <glutil/glutil.h>


CubeMapTexture::CubeMapTexture(const string& posXFilename, const string& negXFilename, const string& posYFilename, const string& negYFilename, const string& posZFilename, const string& negZFilename) {

	//************************************************
	//	Creating a texture ID for the OpenGL texture
	//************************************************
	GLuint textureID;
	glGenTextures(1, &textureID);

	//************************************************
	//	 Load the faces into the cube map texture
	//************************************************
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	loadCubeMapFace(posXFilename, GL_TEXTURE_CUBE_MAP_POSITIVE_X);
	loadCubeMapFace(negXFilename, GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
	loadCubeMapFace(posYFilename, GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
	loadCubeMapFace(negYFilename, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
	loadCubeMapFace(posZFilename, GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
	loadCubeMapFace(negZFilename, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);

	//************************************************
	//			Set filtering parameters
	//************************************************

	//glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	// Sets the type of mipmap interpolation to be used on magnifying and 
	// minifying the active texture. 
	// For cube maps, filtering across faces causes artifacts - so disable filtering
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// In case you want filtering anyway, try this below instead
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // Use tri-linear mip map filtering
	//glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);			  // or replace trilinear mipmap filtering with nicest anisotropic filtering


	CHECK_GL_ERROR();
	m_texture = textureID;

}



void CubeMapTexture::loadCubeMapFace(std::string filename, GLenum face)
{
	ILuint image;
	ilGenImages(1, &image);
	ilBindImage(image);

	bool he = ilLoadImage(filename.c_str());

	if (ilLoadImage(filename.c_str()) == IL_FALSE)   {
	  Logger::logSevere("Failed to load texture " + filename);
		ILenum Error;
		while ((Error = ilGetError()) != IL_NO_ERROR)
			printf("%d: %s\n", Error, iluErrorString(Error));
	}
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	int s = std::max(ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT));
	iluScale(s, s, ilGetInteger(IL_IMAGE_DEPTH));
	glTexImage2D(face, 0, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());
}

void CubeMapTexture::bind(GLenum textureUnit) {
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
}



CubeMapTexture::~CubeMapTexture()
{
}
