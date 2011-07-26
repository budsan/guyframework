#pragma once

#include <vector>
#include <string>
#include <map>
#include <set>

#include "sprite.h"

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

	bool Load(const char* filename);
	bool Save(const char* filename);

	std::set<std::string> getContentFilename();
	void getContentFilename(std::set<std::string> &contentFilename);

private:
	bool ReadSPRITESHEET (int &currentSpritesheet, std::string &line, int lineNum);
	bool ReadFRAME(int currentSpritesheet, std::string &line, int lineNum);
};

class SpriteStatic : public Sprite {
 public:
	SpriteStatic();
	SpriteStatic(SpriteLibrary *data);

	virtual void Update(float GameTime);

	int getFrameID(std::string name);
	bool setLibrary(SpriteLibrary *data);
	bool SelectFrame(std::string name);
	bool SelectFrame(int frameID);

	void getParamsToDraw(Sprite::drawParams &params);
 private:
	SpriteLibrary *data;
	int frameSelected;
};
