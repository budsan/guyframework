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

TextureManager* TextureManager::myInstance = 0;
bool TextureManager::myIsInstanced = false;

TextureManager& TextureManager::Instance()
{
	return *pInstance();
}

TextureManager* TextureManager::pInstance()
{
	if (TextureManager::myInstance == 0)
	{
		myInstance = new TextureManager();
	}
	
	return myInstance;
}

//---------------------------------------------------------------------------//

TextureManager::TextureManager()
{
	myDataPath = std::string("./data/texture/");
	myTextures[NULL_TEXTURE].LoadNullTexture();

	if (!myIsInstanced)
	{
		atexit(TextureManager::DeleteInstance);
		myIsInstanced = true;
	}
}

//---------------------------------------------------------------------------//

TextureManager::~TextureManager()
{
	UnloadTextures();
}

//---------------------------------------------------------------------------//
void TextureManager::UseTexture(std::string _filename)
{
	getTexture(_filename).Bind();
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
}

//---------------------------------------------------------------------------//

const Texture& TextureManager::getTexture(std::string _filename)
{
	std::map<std::string,Texture>::iterator iter = myTextures.find(_filename);

	// La textura ya está cargada, así que la devolvemos
	if(iter != myTextures.end()) return iter->second;

	// Probaremos las rutas posibles para cargar la textura, y ver si alguna
	// opcion nos da una textura valida.
	std::vector<std::string> paths = getPosibleDataPath(_filename);
	int valid = -1;
	for(int i = 0; i < (int)paths.size(); i++)
	{
		std::string &path = paths[i];
		if (path.empty()) return myTextures[NULL_TEXTURE];
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

	if (valid < 0) return myTextures[NULL_TEXTURE];

	// preparamos la estructura y la insertamos
	Texture &tex = myTextures[_filename];
	if (!tex.Load(paths[valid]))
	{
		myTextures.erase(_filename);
		return myTextures[NULL_TEXTURE];
	}

	return tex;
}

//---------------------------------------------------------------------------//

void TextureManager::DeleteTexture(std::string filename)
{
	if(myTextures.find(filename) != myTextures.end())
	{
		GLuint uiID = myTextures[filename].id;
		glDeleteTextures(1,&uiID);
		myTextures.erase(filename);
	}
} 

//---------------------------------------------------------------------------//

unsigned int TextureManager::NumOfTextures()
{
    return myTextures.size();
}

//---------------------------------------------------------------------------//

void TextureManager::UnloadTextures()
{
	myTextures.clear();
}

//---------------------------------------------------------------------------//

void TextureManager::setDataPath(std::string path)
{
	myDataPath = path; 
}

//---------------------------------------------------------------------------//

void TextureManager::DeleteInstance()
{
	if (myInstance) delete myInstance;
}

//---------------------------------------------------------------------------//

std::vector<std::string> TextureManager::getPosibleDataPath(std::string filename)
{
	std::vector<std::string> pathOpts;
	pathOpts.push_back(myDataPath + filename + std::string(".png"));
	pathOpts.push_back(myDataPath + filename);
	pathOpts.push_back(filename + std::string(".png"));
	pathOpts.push_back(filename);

	return pathOpts;
}

