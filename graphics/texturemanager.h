#pragma once

#include "graphics.h"
#include "texture.h"

#include <vector>
#include <map>
#include <string>

class TextureManager
{
public:
	static TextureManager& Instance();
	static TextureManager* pInstance();
	~TextureManager(void);

	void           UseTexture (std::string _sFilename);
	const Texture& getTexture(std::string _sFilename);
	void           DeleteTexture(std::string _sFilename);
	unsigned int   NumOfTextures();
	void           UnloadTextures();
	void           setDataPath(std::string path);

protected:

	TextureManager();
	static void DeleteInstance();
	static TextureManager* myInstance;
	static bool myIsInstanced;

	std::vector<std::string> getPosibleDataPath(std::string filename);
	std::map<std::string, Texture> myTextures;
	std::string myDataPath;
};
