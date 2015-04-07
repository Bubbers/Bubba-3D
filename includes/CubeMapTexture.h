#ifndef __CUBEMAP_H__
#define __CUBEMAP_H__



#include <string>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <IL/il.h>
#include <IL/ilut.h>
#include "Logger.h"

using namespace std;

class CubeMapTexture
{
public:
	CubeMapTexture(const string& posXFilename, const string& negXFilename, const string& posYFilename, const string& negYFilename, const string& posZFilename, const string& negZFilename);
	~CubeMapTexture();

	void load();
	void bind(GLenum textureUnit);

private:
	GLuint m_texture;
	Logger m_logger = Logger::instance();

	void loadCubeMapFace(std::string filename, GLenum face);
	
};

#endif // !__CUBEMAP_H__
