#pragma once

#include "texture.h"

#include <vector>
#include <map>
#include <string>

namespace Guy {

class TextureManager
{
public:
	static TextureManager& instance();
	static TextureManager* ptrInstance();
	~TextureManager(void);

	void           useTexture (std::string _sFilename);
	const Texture& getTexture(std::string _sFilename);
	void           deleteTexture(std::string _sFilename);
	unsigned int   numOfTextures();
	void           unloadTextures();
	void           setDataPath(std::string path);

protected:

	TextureManager();
	static void deleteInstance();
	static TextureManager* s_instance;
	static bool s_isInstanced;

	std::vector<std::string> getPosibleDataPath(std::string filename);
	std::map<std::string, Texture> m_textures;
	std::string m_dataPath;
};

} // namespace Guy
