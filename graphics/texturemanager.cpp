#include "texturemanager.h"

#include <cstdlib>
#include <fstream>
#include <map>
#include <vector>
#include <string>

#include "graphics.h"
#include "settings.h"
#include "log.h"

#define NULL_TEXTURE ""

//---------------------------------------------------------------------------//

TextureManager* TextureManager::m_instance = 0;
bool TextureManager::m_isInstanced = false;

TextureManager& TextureManager::instance()
{
	return *ptrInstance();
}

TextureManager* TextureManager::ptrInstance()
{
	if (TextureManager::m_instance == 0)
	{
		m_instance = new TextureManager();
	}
	
	return m_instance;
}

//---------------------------------------------------------------------------//

TextureManager::TextureManager()
{
	m_dataPath = std::string("./data/texture/");
	m_textures[NULL_TEXTURE].loadNullTexture();

	if (!m_isInstanced)
	{
		atexit(TextureManager::deleteInstance);
		m_isInstanced = true;
	}
}

//---------------------------------------------------------------------------//

TextureManager::~TextureManager()
{
	unloadTextures();
}

//---------------------------------------------------------------------------//
void TextureManager::useTexture(std::string _filename)
{
	getTexture(_filename).bind();
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
}

//---------------------------------------------------------------------------//

const Texture& TextureManager::getTexture(std::string _filename)
{
	std::map<std::string,Texture>::iterator iter = m_textures.find(_filename);

	// La textura ya está cargada, así que la devolvemos
	if(iter != m_textures.end()) return iter->second;

	// Probaremos las rutas posibles para cargar la textura, y ver si alguna
	// opcion nos da una textura valida.
	std::vector<std::string> paths = getPosibleDataPath(_filename);
	int valid = -1;
	for(int i = 0; i < (int)paths.size(); i++)
	{
		std::string &path = paths[i];
		if (path.empty()) return m_textures[NULL_TEXTURE];
		std::ifstream f(path.c_str());
		if (!f.is_open())
		{
#ifdef _DEBUG
			LOG << "Unable to load " << path.c_str() << std::endl;
#endif
			continue;
		}
		valid = i;
		break;
	}

	if (valid < 0) return m_textures[NULL_TEXTURE];

	// preparamos la estructura y la insertamos
	Texture &tex = m_textures[_filename];
	if (!tex.load(paths[valid]))
	{
		m_textures.erase(_filename);
		return m_textures[NULL_TEXTURE];
	}

	return tex;
}

//---------------------------------------------------------------------------//

void TextureManager::deleteTexture(std::string filename)
{
	if(m_textures.find(filename) != m_textures.end())
	{
		m_textures.erase(filename);
	}
} 

//---------------------------------------------------------------------------//

unsigned int TextureManager::numOfTextures()
{
    return m_textures.size();
}

//---------------------------------------------------------------------------//

void TextureManager::unloadTextures()
{
	m_textures.clear();
}

//---------------------------------------------------------------------------//

void TextureManager::setDataPath(std::string path)
{
	m_dataPath = path; 
}

//---------------------------------------------------------------------------//

void TextureManager::deleteInstance()
{
	if (m_instance) delete m_instance;
}

//---------------------------------------------------------------------------//

std::vector<std::string> TextureManager::getPosibleDataPath(std::string filename)
{
	std::vector<std::string> pathOpts;
	pathOpts.push_back(m_dataPath + filename + std::string(".png"));
	pathOpts.push_back(m_dataPath + filename);
	pathOpts.push_back(filename + std::string(".png"));
	pathOpts.push_back(filename);

	return pathOpts;
}

