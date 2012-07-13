#pragma once

#include <vector>
#include <string>
#include <map>
#include <set>
#include "boost/shared_ptr.hpp"

#include "sprite.h"

namespace Guy {

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

	bool load(const char* filename);
	bool save(const char* filename);

	std::set<std::string> getContentFilename();
	void getContentFilename(std::set<std::string> &contentFilename);

private:
	bool readANIM (
		std::string &currentAnimName,
		SpriteAnimTrack *&currentAnimTrack,
		std::string &line, int lineNum);
	bool readFRAME(
		SpriteAnimTrack *&currentAnimTrack,
		std::string &line, int lineNum);
};

class SpriteAnim : public Sprite {
 public:
	SpriteAnim();
	SpriteAnim(boost::shared_ptr<SpriteAnimData> data);

	virtual void update(float deltaTime);

	int getAnimID(std::string name);
	int getLoopsLeft();
	bool setAnimData(boost::shared_ptr<SpriteAnimData> data);
	bool SelectAnim(std::string name);
	bool SelectAnim(int animID);

	void getParamsToDraw(Sprite::drawParams &params);
 private:
	boost::shared_ptr<SpriteAnimData> m_data;
	int m_animSelected;
	int m_loopsLeft;
	int m_frameSelected;
	float m_frameTimeLeft;

	void nextFrame();
};

} // namespace Guy
