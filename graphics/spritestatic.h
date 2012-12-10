#pragma once

#include <vector>
#include <string>
#include <map>
#include <set>
#include "boost/shared_ptr.hpp"

#include "sprite.h"

namespace Guy {

struct SpriteFrame {
	short x, y, w, h; //Rect
	short cx, cy; // Centro del sprite
	unsigned int filename;
};

class SpriteLibrary {
public:
	std::vector<std::string> spritesheetFilenames;
	std::vector<SpriteFrame> frames;

	std::map<std::string, int> spritesheetIndex;
	std::map<std::string, int> frameNames;

	bool load(const char* filename);
	bool Save(const char* filename);
	bool Empty();

	std::set<std::string> getContentFilename();
	void getContentFilename(std::set<std::string> &contentFilename);

private:
	bool ReadSPRITESHEET (int &currentSpritesheet, std::string &line, int lineNum);
	bool ReadFRAME(int currentSpritesheet, std::string &line, int lineNum);
};

class SpriteStatic : public Sprite {
 public:
	SpriteStatic();
	SpriteStatic(boost::shared_ptr<SpriteLibrary> data);

	virtual void update(float deltaTime);

	int getFrameID(std::string name);
	bool setLibrary(boost::shared_ptr<SpriteLibrary> data);
	bool selectFrame(std::string name);
	bool selectFrame(int frameID);

	void getParamsToDraw(Sprite::drawParams &params);
 private:
	boost::shared_ptr<SpriteLibrary> m_data;
	int m_frameSelected;
};

} // namespace Guy
