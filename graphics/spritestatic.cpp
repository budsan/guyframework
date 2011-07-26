#include "spritestatic.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "base/log.h"

SpriteStatic::SpriteStatic() : Sprite()
{
	data = NULL;
	frameSelected  = 0;
}

SpriteStatic::SpriteStatic(SpriteLibrary *_data) : Sprite()
{
	setLibrary(_data);
}

void SpriteStatic::Update(float GameTime)
{

}


bool SpriteStatic::setLibrary(SpriteLibrary* _data)
{
	if (_data->frames.size() == 0) return false;
	data = _data;
	frameSelected = 0;

	return true;
}

void SpriteStatic::getParamsToDraw(Sprite::drawParams &params)
{
	if (data == NULL) {
		params.filename = NULL;
		return;
	}
	const SpriteFrame& fram = data->frames[frameSelected];
	params.filename = data->spritesheetFilenames[fram.filename].c_str();
	params.x  = fram.x;
	params.y  = fram.y;
	params.w  = fram.w;
	params.h  = fram.h;
	params.cx = fram.cx;
	params.cy = fram.cy;
}

int SpriteStatic::getFrameID(std::string name)
{
	if (data == NULL) return -1;
	std::map<std::string, int>::iterator it = data->frameNames.find(name);
	if (it == data->frameNames.end()) return -1;
	return (int)it->second;
}

bool SpriteStatic::SelectFrame(std::string name)
{
	int id = getFrameID(name);
	if (id < 0) return false;
	return SelectFrame(id);
}

bool SpriteStatic::SelectFrame(int frameID)
{
	if (data == NULL) return false;
	if (data->frames.size() == 0) return false;
	frameSelected  = frameID;
	return true;
}

bool SpriteLibrary::Load(const char* filename)
{
	std::ifstream in(filename);

	if (in.fail())
	{
		LOG << "Error: " << filename << " doesn't exist." << std::endl;
		return false;
	}

	int currentSpritesheet = -1;
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
		if (param == std::string("SPRITESHEET") ) {
			ReadSPRITESHEET(currentSpritesheet, line, lineNum);
		}
		else if (currentSpritesheet == -1) {
			LOG << "Error "<<lineNum<<": spritesheet filename undefined yet." << std::endl;
		}
		else {
			ReadFRAME(currentSpritesheet, line, lineNum);
		}
	}
	
	return true;
}

bool SpriteLibrary::Save(const char* filename)
{
	std::ofstream out(filename, std::ios_base::out | std::ios_base::trunc);

	if (!out.is_open())
	{
		LOG<<"Error: Cannot open for save filename " << filename << std::endl;
		return false;
	}

	for (unsigned int i = 0; i < spritesheetFilenames.size(); i++)
	{
		out << "SPRITESHEET \"" << spritesheetFilenames[i] << "\"" << std::endl;
		std::map<std::string, int>::iterator it = frameNames.begin();
		for (; it != frameNames.end(); it++)
		{
			SpriteFrame &frame = frames[it->second];
			if (frame.filename != i) continue;
			out << frame.x <<" "<< frame.y <<" "
			    << frame.w <<" "<< frame.h <<" "
			    << frame.cx <<" "<< frame.cy << " \""
			    << it->first << "\"" << std::endl;
		}
	}

	return true;
}

bool SpriteLibrary::ReadSPRITESHEET(int &currentSpritesheet, std::string &line, int lineNum)
{
	//Leemos el nombre de fichero del spritesheet
	size_t quoteStart, quoteEnd = 0;

	quoteStart = line.find('\"', quoteEnd);
	if (quoteStart == std::string::npos) {
		LOG<<"Error "<< lineNum <<": SPRITESHEET filename not found. Skipping line."<<std::endl;
		return false;
	}
	quoteStart++;

	quoteEnd = line.find('\"', quoteStart);
	if (quoteEnd == std::string::npos) {
		LOG<<"Error "<< lineNum <<": SPRITESHEET filename incomplete. Skipping line."<<std::endl;
		return false;
	}

	std::string spritesheetFilename = line.substr(quoteStart, quoteEnd - quoteStart);
	std::map<std::string, int>::iterator it = spritesheetIndex.find(spritesheetFilename);
	if (it == spritesheetIndex.end())
	{
		currentSpritesheet = spritesheetFilenames.size();
		spritesheetFilenames.push_back(spritesheetFilename);
		spritesheetIndex[spritesheetFilename] = currentSpritesheet;
	}
	else
	{
		currentSpritesheet = it->second;
	}

	return true;
}

bool SpriteLibrary::ReadFRAME(int currentSpritesheet, std::string &line, int lineNum)
{
		//TODO CHECK ERRORS
		std::stringstream sline(line);
		int begin = sline.tellg();

		SpriteFrame frame;
		frame.filename = currentSpritesheet;

		short *info[7] = { 
			&frame.x, &frame.y, 
			&frame.w, &frame.h, 
			&frame.cx, &frame.cy
		};

		for (int i = 0 ; i < 6; ++i)
		{
			if (!(sline >> *info[i])) 
			{
				LOG<<"Error "<< lineNum <<": FRAME incorrect or incomplete. Skipping line."<<std::endl;
				return false;
			}
		}

		//Leemos el nombre del sprite
		size_t quoteStart, quoteEnd = int(sline.tellg())-begin;

		quoteStart = line.find('\"', quoteEnd);
		if (quoteStart == std::string::npos) {
			LOG<<"Error "<< lineNum <<": FRAME name not found. Skipping line."<<std::endl;
			return false;
		}
		quoteStart++;

		quoteEnd = line.find('\"', quoteStart);
		if (quoteEnd == std::string::npos) {
			LOG<<"Error "<< lineNum <<": FRAME name incomplete. Skipping line."<<std::endl;
			return false;
		}

		std::string framename = line.substr(quoteStart, quoteEnd - quoteStart);
		std::map<std::string, int>::iterator it = frameNames.find(framename);
		if ( it != frameNames.end() ) //EXISTS
		{
			LOG<<"Warning "<< lineNum <<": FRAME name redefined. Overriding attributes."<<std::endl;
			frames[it->second] = frame;
		}
		else
		{
			frames.push_back(frame);
			frameNames[framename] = frames.size()-1;
		}

		return true;
}
