#include "spriteanim.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "base/log.h"

SpriteAnim::SpriteAnim() : Sprite() 
{
	data = NULL;
	animSelected  = 0;
	loopsLeft     = 0;
	frameSelected = 0;
	frameTimeLeft = 0;
}

SpriteAnim::SpriteAnim(SpriteAnimData *_data) : Sprite() 
{
	setAnimData(_data);
}

void SpriteAnim::Update(float GameTime)
{
	if (data == NULL) return;

	float time = GameTime*1000;
	while(time != 0)
	{
		if (time <= frameTimeLeft) {
			frameTimeLeft -= time;
			time = 0;
		}
		else {
			time -= frameTimeLeft;
			NextFrame();
		}
	}
}

void SpriteAnim::NextFrame()
{
	const SpriteAnimTrack& anim = data->animations[animSelected];
	if (frameSelected+1 >= (int) anim.frames.size()) {
		if (loopsLeft  > 0) --loopsLeft;
		if (loopsLeft != 0) frameSelected = 0;
	}
	else frameSelected++;
	frameTimeLeft = anim.frames[frameSelected].time;
}

bool SpriteAnim::setAnimData(SpriteAnimData* _data)
{
	if (_data->animations.size() == 0) return false;

	data = _data;
	if (!SelectAnim(0)) {
		data = NULL;
		return false;
	}

	return true;
}

void SpriteAnim::getParamsToDraw(Sprite::drawParams &params)
{
	if (data == NULL) {
		params.filename = NULL;
		return;
	}

	const SpriteAnimTrack& anim = data->animations[animSelected];
	const SpriteAnimFrame& fram = anim.frames[frameSelected];

	params.filename = anim.spritesheetFilename.c_str();
	params.x  = fram.x;
	params.y  = fram.y;
	params.w  = fram.w;
	params.h  = fram.h;
	params.cx = fram.cx;
	params.cy = fram.cy;
}

int SpriteAnim::getAnimID(std::string name)
{
	if (data == NULL) return -1;
	std::map<std::string, int>::iterator it = data->animNames.find(name);
	if (it == data->animNames.end()) return -1;
	return (int)it->second;
}

int SpriteAnim::getLoopsLeft()
{
	return loopsLeft;
}

bool SpriteAnim::SelectAnim(std::string name)
{
	int id = getAnimID(name);
	if (id < 0) return false;
	return SelectAnim(id);
}

bool SpriteAnim::SelectAnim(int animID)
{
	if (data == NULL) return false;
	if (data->animations[animID].frames.size() == 0) return false;

	animSelected  = animID;
	const SpriteAnimTrack& anim = data->animations[animSelected];
	loopsLeft = anim.numOfLoops;
	if (loopsLeft == 0) loopsLeft = -1;

	frameSelected = 0;
	frameTimeLeft = anim.frames[frameSelected].time;

	return true;
}

bool SpriteAnimData::Load(const char* filename)
{
	std::ifstream in(filename);

	if (in.fail())
	{
		LOG << "Error: " << filename << " doesn't exist." << std::endl;
		return false;
	}


	std::string currentAnimName;
	SpriteAnimTrack *currentAnimTrack = NULL;
	std::string line;
	int lineNum = 0;
	
	while( getline(in, line) )
	{
		//Ignore comments
		size_t comment_pos = line.find('#', 0);
		if (comment_pos != std::string::npos) {
			line.erase(comment_pos, std::string::npos);
		}
		
		std::string param;
		std::stringstream sline(line);
		
		lineNum = lineNum + 1;
		if (!(sline >> param)) {
			continue; //Ignore blank line
		}
		if (param == std::string("ANIM") ) {
			ReadANIM(currentAnimName, currentAnimTrack, line, lineNum);
		}
		else if (currentAnimTrack == NULL) {
			LOG << "Error "<<lineNum<<": Animation name undefined yet." << std::endl;
		}
		else {
			ReadFRAME(currentAnimTrack, line, lineNum);
		}
	}
	
	std::vector<SpriteAnimTrack>::iterator it = animations.begin();
	while(it != animations.end()) {
		if (it->frames.size() == 0) {
			LOG << "Warning: Animation with no frames. Deleting." << std::endl;
			animations.erase(it);
		}
		else ++it;
	}

	if (animations.size() == 0)
	{
		LOG << "Error: Zero valid animations loaded." << std::endl;
		return false;
	}

	return true;
}

bool SpriteAnimData::Save(const char* filename)
{
	std::ofstream out(filename, std::ios_base::out | std::ios_base::trunc);

	std::vector<std::string> aux(animations.size());
	std::map<std::string, int>::iterator it = animNames.begin();
	for(;it != animNames.end(); it++) {
		aux[it->second] = it->first;
	}

	for (unsigned int i = 0; i < animations.size(); ++i)
	{
		SpriteAnimTrack &track = animations[i];
		out << "ANIM \"" << aux[i] << "\" \"" << track.spritesheetFilename << "\" " << track.numOfLoops << std::endl;
		for (unsigned int j = 0; j < track.frames.size(); ++j)
		{
			SpriteAnimFrame &frame = track.frames[j];
			out << frame.x <<" "<< frame.y <<" "
				<< frame.w <<" "<< frame.h <<" "
				<< frame.cx <<" "<< frame.cy <<" "
				<< frame.time << std::endl;
		}
	}
	return true;
}

bool SpriteAnimData::ReadANIM(std::string &currentAnimName, SpriteAnimTrack *&currentAnimTrack, std::string &line, int lineNum)
{
	//Leemos el nombre de la animacion situado entre comillas.
	size_t quoteStart = line.find('\"', 0);
	if (quoteStart == std::string::npos) {
		LOG<<"Error "<< lineNum <<": ANIM name not found."<<std::endl;
		return false;
	}
	quoteStart++;

	size_t quoteEnd = line.find('\"', quoteStart);
	if (quoteEnd == std::string::npos) {
		LOG<<"Error "<< lineNum <<": ANIM name incomplete."<<std::endl;
		return false;
	}

	std::string AnimName = line.substr(quoteStart, quoteEnd - quoteStart);
	quoteEnd++;

	quoteStart = line.find('\"', quoteEnd);
	if (quoteStart == std::string::npos) {
		LOG<<"Error "<< lineNum <<": ANIM spritesheet filename not found. Skipping line."<<std::endl;
		return false;
	}
	quoteStart++;

	quoteEnd = line.find('\"', quoteStart);
	if (quoteEnd == std::string::npos) {
		LOG<<"Error "<< lineNum <<": ANIM spritesheet filename incomplete. Skipping line."<<std::endl;
		return false;
	}

	std::string spritesheetFilename = line.substr(quoteStart, quoteEnd - quoteStart);
	quoteEnd++;

	std::string lineLeft = line.substr(quoteEnd, line.size() - quoteEnd);
	std::stringstream sline(lineLeft);

	int loops = 0;
	if (!(sline >> loops)) {
		LOG<<"Error "<< lineNum <<": ANIM num of loops not found.  Skipping line."<<std::endl;
		return false;
	}

	std::map<std::string, int>::iterator it = animNames.find(AnimName);
	if ( it != animNames.end() ) //EXISTS
	{
		LOG<<"Warning "<< lineNum <<": Animation name redefined. Overriding attributes."<<std::endl;
		currentAnimTrack = &animations[it->second];
	}
	else 
	{
		int ID = (int)animations.size();
		animations.push_back(SpriteAnimTrack());
		animNames[AnimName] = ID;
		currentAnimTrack = &animations[ID];
	}

	currentAnimName = AnimName;
	currentAnimTrack->numOfLoops = loops;
	currentAnimTrack->spritesheetFilename = spritesheetFilename;

	return true;
}

bool SpriteAnimData::ReadFRAME(SpriteAnimTrack *&currentAnimTrack, std::string &line, int lineNum)
{
		//TODO CHECK ERRORS
		std::stringstream sline(line);
		SpriteAnimFrame frame;
		short *info[7] = { 
			&frame.x, &frame.y, 
			&frame.w, &frame.h, 
			&frame.cx, &frame.cy,
			&frame.time
		};

		for (int i = 0 ; i < 7; ++i) 
		{
			if (!(sline >> *info[i])) 
			{
				LOG<<"Error "<< lineNum <<": FRAME incorrect or incomplete. Skipping line."<<std::endl;
				return false;
			}
		}

		if(frame.time < 1)
		{
			LOG << "Error "<< lineNum <<": time isn't bigger than 0, setting time to 1." <<std::endl;
			frame.time = 1;
		}
		currentAnimTrack->frames.push_back(frame);
		return true;
}

std::set<std::string> SpriteAnimData::getContentFilename()
{
	std::set<std::string> contentFilename;
	std::vector<SpriteAnimTrack>::iterator it = animations.begin();
	for(;it != animations.end(); ++it) {
		contentFilename.insert(it->spritesheetFilename);
	}
	return contentFilename;
}
void SpriteAnimData::getContentFilename(std::set<std::string> &contentFilename)
{
	std::vector<SpriteAnimTrack>::iterator it = animations.begin();
	for(;it != animations.end(); ++it) {
		contentFilename.insert(it->spritesheetFilename);
	}
}
