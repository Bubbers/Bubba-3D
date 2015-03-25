#include <iostream>
#include <fstream>
#include <sstream>
#include <float.h>
#include "GL/glew.h"
#include "GL/glut.h"
#include <IL/il.h>
#include <IL/ilut.h>
#include "OBJModel.h"
#include "glutil.h"
#include <stdlib.h>

#ifdef _MSC_VER
#	define FORCE_INLINE __forceinline
#	define FLATTEN 
#elif defined(__GNUC__) && __GNUC__ > 4
#	define FORCE_INLINE __attribute__((always_inline))
#	define FLATTEN __attribute__((flatten))
#else 
#	define FORCE_INLINE inline
#	define FLATTEN
#endif //_MSC_VER || __GNUC__ > 4


using namespace std;
using namespace chag;

OBJModel::OBJModel(void)
{
}

OBJModel::~OBJModel(void)
{
}

void OBJModel::load(std::string fileName)
{
	std::ifstream file;

  // ensure only one type of slashes...
  std::replace(fileName.begin(), fileName.end(), '\\', '/');

  file.open(fileName.c_str(), std::ios::binary);
	if (!file)
	{
		cout << "Error in openening file '" << fileName << "'" << endl;
		exit(1);
	}
	cout << "Loading OBJ file: '" << fileName << "'..." << endl;
  std::string basePath = fileName.substr(0, fileName.find_last_of('/')) + "/";
	loadOBJ(file, basePath);
	cout << " vertex count: " << getNumVerts() << endl;
}



void OBJModel::setMaterialDiffuseTextureId(std::string matName, int textureId)
{
  m_materials[matName].diffuse_map_id = textureId;
}



size_t OBJModel::getNumVerts()
{
	size_t result = 0;
	for (size_t i = 0; i < m_chunks.size(); ++i)
	{
		Chunk &chunk = m_chunks[i];
		result += chunk.m_positions.size();
	}
	return result;
}


struct ObjTri
{
	int v[3];
	int t[3];
	int n[3];
};



// The next step is to create a dedicated lexer (flex takes about 40% of total), where we can make 
// use of the line by line structure of the file to optimize.
class ObjLexer
{
public:
  enum { s_bufferLength = 512 };

  ObjLexer(istream* input) : 
    m_input(input), 
    m_bufferPos(0), 
    m_bufferEnd(0) 
  { 
  }

  enum Tokens
  {
    T_Eof      = 0,
    T_MtlLib   = 'm' << 8 | 't',
    T_UseMtl   = 'u' << 8 | 's',
    T_Face     = 'f' << 8 | ' ', // line starts with 'f' followed by ' '
    T_Face2    = 'f' << 8 | '\t', // line starts with 'f' followed by '\t'
    T_Vertex   = 'v' << 8 | ' ', // line starts with 'v' followed by ' '
    T_Vertex2  = 'v' << 8 | '\t', // line starts with 'v' followed by '\t'
    T_Normal   = 'v' << 8 | 'n',
    T_TexCoord = 'v' << 8 | 't',
  };

  FORCE_INLINE int fillBuffer()
  {
    if (m_bufferPos >= m_bufferEnd)
    {
      m_input->read(m_buffer, s_bufferLength);
      m_bufferEnd = int(m_input->gcount());
      m_bufferPos = 0;
    }
    return m_bufferEnd != 0;
  }

  FORCE_INLINE int nextChar()
  {
    if (fillBuffer())
    {
      return m_buffer[m_bufferPos++];
    }
    return 0;
  }

  int firstLine()
  {
    // read the first line token.
    return nextChar() << 8 | nextChar();
  }

  FORCE_INLINE int nextLine()
  {
    // scan to end of line...
    while('\n' != nextChar())
    {
    }
    while(matchChar('\n') || matchChar('\r'))
    {
    }
    // Or: convert next 2 chars to token (16 bit), can be mt, us, vn, vt, v , v\t, f , f\t, unknown
    return nextChar() << 8 | nextChar();

/*    switch()
    {
    case 'm':
      return (match("tllib") && matchWs()) ? OT_MtlLib : OT_Unknown;
    case 'u':
      return (match("semtl") && matchWs()) ? OT_UseMtl : OT_Unknown;
    case 'f':
      return matchWs() ? OT_Face : OT_Unknown;
    case 'v':
      if (matchWs())
        return OT_Vertex;
      if (match("n") && matchWs())
        return OT_Normal;
      if (match("t") && matchWs())
        return OT_TexCoord;
      break;
    default:
      break;
    };
    return OT_Unknown;*/
  }


  FORCE_INLINE bool match(const char s[], const size_t l)
  {
    for (int i = 0; fillBuffer() && i < int(l) - 1; ++i)
    {
      if (s[i] != m_buffer[m_bufferPos])
      {
        return false;
      }
      else
      {
        ++m_bufferPos;
      }
    }
    return true;
  }
  FORCE_INLINE bool matchString(std::string &str)
  {
    while (fillBuffer() && !isspace(m_buffer[m_bufferPos]))
    {
      str.push_back(m_buffer[m_bufferPos++]);
    }
    return !str.empty();
  }

  FORCE_INLINE bool matchFloat(float &result)
  {
    bool found = false;
    result = 0.0f;
    float sign = 1.0f;
    if (matchChar('-'))
    {
      sign = -1.0f;
      found = true;
    }
    char c;
    while (fillBuffer() && myIsDigit(c = m_buffer[m_bufferPos]))
    {
      result = result * 10.0f + float(c - '0'); 
      ++m_bufferPos;
      found = true;
    }
    float frac = 0.1f;
    if (matchChar('.'))
    {
      char c;
      while (fillBuffer() && myIsDigit(c = m_buffer[m_bufferPos]))
      {
        result += frac * float(c - '0'); 
        ++m_bufferPos;
        frac *= 0.1f;
      }
      found = true;
    }
    if (matchChar('e') || matchChar('E'))
    {
      float expSign = matchChar('-') ? -1.0f : 1.0f;
      int exp = 0;
      if (matchInt(exp))
      {
        result = result * powf(10.0f, float(exp) * expSign);
      }
    }
    result *= sign;
    return found;
  }

  FORCE_INLINE bool myIsDigit(char c)
  {
    return ((unsigned int)(c) - (unsigned int)('0') < 10U);
  }

  FORCE_INLINE bool matchInt(int &result)
  {
    bool found = false;
    result = 0;
    char c;
    while (fillBuffer() && myIsDigit(c = m_buffer[m_bufferPos]))// isdigit(m_buffer[m_bufferPos]))
    {
      result = result * 10 + int(c - '0'); 
      ++m_bufferPos;
      found = true;
    }
    return found;
  }
  FORCE_INLINE bool matchChar(int matchTo)
  {
    if (fillBuffer() && m_buffer[m_bufferPos] == matchTo)
    {
      m_bufferPos++;
      return true;
    }
    return false;
  }

  FORCE_INLINE bool matchWs(bool optional = false)
  {
    bool found = false;
    while (fillBuffer() && 
      (m_buffer[m_bufferPos] == ' ' || m_buffer[m_bufferPos] == '\t'))
    {
      found = true;
      m_bufferPos++;
    }
    return found || optional;
  }
  istream* m_input;
  char m_buffer[s_bufferLength];
  int m_bufferPos;
  int m_bufferEnd;
};

FORCE_INLINE static bool parseFaceIndSet(ObjLexer &lexer, ObjTri &t, int v)
{
  t.v[v] = -1;
  t.t[v] = -1;
  t.n[v] = -1;

  if(lexer.matchWs(true)
    && lexer.matchInt(t.v[v])
    &&   lexer.matchChar('/')
    &&   (lexer.matchInt(t.t[v]) || true)  // The middle index is optional!
    &&   lexer.matchChar('/')
    &&   lexer.matchInt(t.n[v]))
  {
    // need to adjust for silly obj 1 based indexing
    t.v[v] -= 1;
    t.t[v] -= 1;
    t.n[v] -= 1;
    return true;
  }
  return false;
}



void FLATTEN OBJModel::loadOBJ(ifstream &file, std::string basePath) 
{
	std::vector<float3> positions;
	std::vector<float3> normals;
	std::vector<float2> uvs;
	std::vector<ObjTri> tris;

  positions.reserve(256 * 1024);
  normals.reserve(256 * 1024);
  uvs.reserve(256 * 1024);
  tris.reserve(256 * 1024);

	std::vector<std::pair<std::string, size_t> > materialChunks;

	cout << "  Reading data..." << endl << flush;

  ObjLexer lexer(&file);
  for(int token = lexer.firstLine(); token != ObjLexer::T_Eof; token = lexer.nextLine())
  {
    switch(token)
    {
    case ObjLexer::T_MtlLib: 
      {
        string materialFile;
        if(lexer.match("llib", sizeof("llib")) && lexer.matchWs() && lexer.matchString(materialFile))
        {
          loadMaterials(basePath + materialFile, basePath);
        }
        break;
      }
    case ObjLexer::T_UseMtl: 
    {
      string materialName;
      if(lexer.match("emtl", sizeof("emtl")) && lexer.matchWs() && lexer.matchString(materialName))
      {
        if (materialChunks.size() == 0 || (*materialChunks.rbegin()).first != materialName)
        {
          materialChunks.push_back(std::make_pair(materialName, tris.size()));
        }
      }
    }
    break;
    case ObjLexer::T_Vertex:
    case ObjLexer::T_Vertex2:
      {
        float3 p;
        if (lexer.matchWs(true)
          && lexer.matchFloat(p.x)
          && lexer.matchWs()
          && lexer.matchFloat(p.y)
          && lexer.matchWs()
          && lexer.matchFloat(p.z))
        {
          positions.push_back(p);
        }
		  }
      break;
    case ObjLexer::T_Normal: 
      {
        float3 n = { 0.0f, 0.0f, 0.0f };
        if (lexer.matchWs(true)
          && lexer.matchFloat(n.x)
          && lexer.matchWs()
          && lexer.matchFloat(n.y)
          && lexer.matchWs()
          && lexer.matchFloat(n.z))
        {
          normals.push_back(n);
        }
      }
      break;
    case ObjLexer::T_TexCoord: 
      {
        float2 t;
        if (lexer.matchWs(true)
          && lexer.matchFloat(t.x)
          && lexer.matchWs()
          && lexer.matchFloat(t.y))
        {
          uvs.push_back(t);
        }
      }
      break;
    case ObjLexer::T_Face: 
    case ObjLexer::T_Face2:
      {
        ObjTri t;
        // parse vert 0 and 1
        if (parseFaceIndSet(lexer, t, 0) && parseFaceIndSet(lexer, t, 1))
        {
          // let any more produce one more triangle
          while(parseFaceIndSet(lexer, t, 2))
          {
            // kick tri,
   				  tris.push_back(t);
            // the make last vert second (this also keeps winding the same).
				    t.n[1] = t.n[2];
				    t.t[1] = t.t[2];
				    t.v[1] = t.v[2];
          }
        }
			}
      break;
    default:
      break;
    };
  }

	cout << "  done." << endl;

	cout << "  Shuffling..." << flush;
	// Reshuffle the normals and vertices to be unique.
	for (size_t i = 0; i < materialChunks.size(); ++i)
	{
		Chunk chunk;
		chunk.material = &m_materials[materialChunks[i].first];
		const size_t start = materialChunks[i].second;
		const size_t end = i + 1 < materialChunks.size() ? materialChunks[i + 1].second : tris.size();

    chunk.m_normals.resize(3 * (end - start));
    chunk.m_positions.resize(3 * (end - start));
    chunk.m_uvs.resize(3 * (end - start));

    for (size_t k = start; k < end; ++k)
    {
      for (int j = 0; j < 3; ++j)
      {
        chunk.m_normals[(k  - start) * 3 + j] = (normals[tris[k].n[j]]);
        chunk.m_positions[(k  - start) * 3 + j] = (positions[tris[k].v[j]]);
				if(tris[k].t[j] != -1)
        {
          chunk.m_uvs[(k  - start) * 3 + j] = (uvs[tris[k].t[j]]);
        }
      }
    }

#if 0
    printf("// mat %s\n", materialChunks[i].first.c_str());
    printf("float3 normals[] = {\n");
		for (size_t index = start; index < end; ++index)
		{
      printf("{ %f, %f, %f },\n", normals[tris[index].n[0]].x, normals[tris[index].n[0]].y, normals[tris[index].n[0]].z);
      printf("{ %f, %f, %f },\n", normals[tris[index].n[1]].x, normals[tris[index].n[1]].y, normals[tris[index].n[1]].z);
      printf("{ %f, %f, %f },\n", normals[tris[index].n[2]].x, normals[tris[index].n[2]].y, normals[tris[index].n[2]].z);
    }
    printf("};\n");

    
    printf("float3 positions[] = {\n");
		for (size_t index = start; index < end; ++index)
		{
      printf("{ %f, %f, %f },\n", positions[tris[index].v[0]].x, positions[tris[index].v[0]].y, positions[tris[index].v[0]].z);
      printf("{ %f, %f, %f },\n", positions[tris[index].v[1]].x, positions[tris[index].v[1]].y, positions[tris[index].v[1]].z);
      printf("{ %f, %f, %f },\n", positions[tris[index].v[2]].x, positions[tris[index].v[2]].y, positions[tris[index].v[2]].z);
    }
    printf("};\n");
    
    printf("float3 texCoords[] = {\n");
		for (size_t index = start; index < end; ++index)
		{
      printf("{ %f, %f },\n", uvs[tris[index].t[0]].x, uvs[tris[index].t[0]].y);
      printf("{ %f, %f },\n", uvs[tris[index].t[1]].x, uvs[tris[index].t[1]].y);
      printf("{ %f, %f },\n", uvs[tris[index].t[2]].x, uvs[tris[index].t[2]].y);
    }
    printf("};\n");
#endif
    m_chunks.push_back(chunk);
	}
	cout << "done." << endl;

	// lastly we could look out for duplicates and compact the array down again, if we would.

	// Now, create a Vertex Array Object per chunk and be done with it
	for (size_t i = 0; i < m_chunks.size(); ++i)
	{
		Chunk &chunk = m_chunks[i];
		glGenVertexArrays(1, &chunk.m_vaob); 
		glBindVertexArray(chunk.m_vaob);

		glGenBuffers(1, &chunk.m_positions_bo); 
		glBindBuffer(GL_ARRAY_BUFFER_ARB, chunk.m_positions_bo);
		glBufferData(GL_ARRAY_BUFFER_ARB, chunk.m_positions.size() * sizeof(float3), 
			&chunk.m_positions[0].x, GL_STATIC_DRAW); 
		glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);	
		glEnableVertexAttribArray(0);

		glGenBuffers(1, &chunk.m_normals_bo); 
		glBindBuffer(GL_ARRAY_BUFFER_ARB, chunk.m_normals_bo);
		glBufferData(GL_ARRAY_BUFFER_ARB, chunk.m_normals.size() * sizeof(float3), 
			&chunk.m_normals[0].x, GL_STATIC_DRAW); 
		glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, 0);	
		glEnableVertexAttribArray(1);

		if(chunk.m_uvs.size() > 0){
			glGenBuffers(1, &chunk.m_uvs_bo); 
			glBindBuffer(GL_ARRAY_BUFFER_ARB, chunk.m_uvs_bo);
			glBufferData(GL_ARRAY_BUFFER_ARB, chunk.m_uvs.size() * sizeof(float2), 
				&chunk.m_uvs[0].x, GL_STATIC_DRAW); 
			glVertexAttribPointer(2, 2, GL_FLOAT, false, 0, 0);	
		}
		glEnableVertexAttribArray(2);
	}		
}



void OBJModel::render()
{
	CHECK_GL_ERROR();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	for (size_t i = 0; i < m_chunks.size(); ++i)
	{
		CHECK_GL_ERROR();
		Chunk &chunk = m_chunks[i];
		if(chunk.material->diffuse_map_id != -1){
			glActiveTexture(GL_TEXTURE0);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, chunk.material->diffuse_map_id);
		}

		GLint current_program = 0; 
		glGetIntegerv(GL_CURRENT_PROGRAM, &current_program);
		glUniform1i(glGetUniformLocation(current_program, "has_diffuse_texture"), chunk.material->diffuse_map_id != -1);
		glUniform3fv(glGetUniformLocation(current_program, "material_diffuse_color"), 1, &chunk.material->diffuseColor.x);
		glUniform3fv(glGetUniformLocation(current_program, "material_specular_color"), 1, &chunk.material->specularColor.x);
		glUniform3fv(glGetUniformLocation(current_program, "material_ambient_color"), 1, &chunk.material->ambientColor.x);
		glUniform3fv(glGetUniformLocation(current_program, "material_emissive_color"), 1, &chunk.material->emissiveColor.x);
		glUniform1f(glGetUniformLocation(current_program, "material_shininess"), chunk.material->specularExponent);
		CHECK_GL_ERROR();

		glBindVertexArray(chunk.m_vaob);
		glDrawArrays(GL_TRIANGLES, 0, (GLsizei)chunk.m_positions.size());
		CHECK_GL_ERROR();
	}
	glPopAttrib();
	CHECK_GL_ERROR();
}

void OBJModel::loadMaterials(std::string fileName, std::string basePath )
{
	ifstream file;
	file.open(fileName.c_str());
	if (!file)
	{
		cout << "Error in openening file";
		exit(1);
	}
	std::string currentMaterial("");

	std::string lineread;
	while(std::getline(file, lineread)) // Read line by line
	{
		//cout << lineread << endl; 
		stringstream ss(lineread); 
		std::string firstword; 
		ss >> firstword;
		std::transform(firstword.begin(), firstword.end(), firstword.begin(), ::tolower);

		if(firstword == "newmtl")
		{
			ss >> currentMaterial;
			Material m = 
			{ 
				{ 0.5f, 0.5f, 0.5f, 1.0f }, 
				{ 0.5f, 0.5f, 0.5f, 1.0f }, 
				{ 0.5f, 0.5f, 0.5f, 1.0f }, 
				{ 0.0f, 0.0f, 0.0f, 1.0f }, 
				20.0f,
				-1 
			};
			m_materials[currentMaterial] = m;
			m_materials[currentMaterial].diffuse_map_id = -1; 
		}
		else if(firstword == "ka")
		{
			float4 color;
			ss >> color.x >> color.y >> color.z;
			m_materials[currentMaterial].ambientColor = color;
		}
		else if(firstword == "kd")
		{
			float4 color;
			ss >> color.x >> color.y >> color.z;
			m_materials[currentMaterial].diffuseColor = color;
		}
		else if(firstword == "ks")
		{
			float4 color;
			ss >> color.x >> color.y >> color.z;
			m_materials[currentMaterial].specularColor = color;
		}
		else if(firstword == "ke")
		{
			float4 color;
			ss >> color.x >> color.y >> color.z;
			m_materials[currentMaterial].emissiveColor = color;
		}
		else if(firstword == "ns")
		{
			ss >> m_materials[currentMaterial].specularExponent;
		}
		else if(firstword == "map_kd")
		{
			std::string fileName; 
			ss >> fileName;
			m_materials[currentMaterial].diffuse_map_id = (int) loadTexture(basePath + fileName, basePath);
		}
	}
}

unsigned int OBJModel::loadTexture(std::string fileName, std::string /*basePath*/ )
{
	std::replace(fileName.begin(), fileName.end(), '\\', '/');

  ILuint image = ilGenImage();
  ilBindImage(image);
	CHECK_GL_ERROR();

  if(ilLoadImage(fileName.c_str()) == IL_FALSE)   
  {
    std::cout << "Failed to load texture: '" << fileName << "'" << std::endl;
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
  glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());
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
	cout << "    Loaded texture '" << fileName << "', (" << ilGetInteger(IL_IMAGE_WIDTH) << "x" << ilGetInteger(IL_IMAGE_HEIGHT) << ")" << endl; 
	glBindTexture(GL_TEXTURE_2D, 0);
	CHECK_GL_ERROR();
	return texid;
}
