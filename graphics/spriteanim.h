#pragma once

#include <vector>
#include <string>
#include <map>
#include <set>

#include "sprite.h"

struct SpriteAnimFrame {
	short x, y, w, h; //Rect
	short cx, cy; // Centro del sprite
	short time; //Milisecs
};

struct SpriteAnimTrack {
	short numOfLoops; //0 INFINITE LOOP, 1 ONCE, 2 TWICE, AND SO.
	std::string spritesheetFilename;
	std::vector<SpriteAnimFrame> frames;
};

class SpriteAnimData {
public:
	std::vector<SpriteAnimTrack> animations;
	std::map<std::string, int> animNames; // Nombre -> posicion en el vector "animations"

	bool Load(const char* filename);
	bool Save(const char* filename);

	std::set<std::string> getContentFilename();
	void getContentFilename(std::set<std::string> &contentFilename);

private:
	bool ReadANIM (
		std::string &currentAnimName,
		SpriteAnimTrack *&currentAnimTrack,
		std::string &line, int lineNum);
	bool ReadFRAME(
		SpriteAnimTrack *&currentAnimTrack,
		std::string &line, int lineNum);
};

class SpriteAnim : public Sprite {
 public:
	SpriteAnim();
	SpriteAnim(SpriteAnimData *data);

	virtual void Update(float GameTime);

	int getAnimID(std::string name);
	int getLoopsLeft();
	bool setAnimData(SpriteAnimData *data);
	bool SelectAnim(std::string name);
	bool SelectAnim(int animID);

	void getParamsToDraw(Sprite::drawParams &params);
 private:
	SpriteAnimData *data;
	int animSelected;
	int loopsLeft;
	int frameSelected;
	float frameTimeLeft;

	void NextFrame();
};
