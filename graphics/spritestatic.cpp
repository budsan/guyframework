#include "spritestatic.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "log.h"

SpriteStatic::SpriteStatic() : Sprite(), m_data()
{
	m_frameSelected  = 0;
}

SpriteStatic::SpriteStatic(boost::shared_ptr<SpriteLibrary> _data) : Sprite()
{
	setLibrary(_data);
}

void SpriteStatic::update(float deltaTime)
{

}


bool SpriteStatic::setLibrary(boost::shared_ptr<SpriteLibrary> _data)
{
	if(_data == NULL)
	{
		m_data.reset();
		return true;
	}

	if (_data->frames.size() == 0) return false;
	m_data = _data;
	m_frameSelected = 0;

	return true;
}

void SpriteStatic::getParamsToDraw(Sprite::drawParams &params)
{
	if (m_data == NULL) {
		params.filename = NULL;
		return;
	}
	const SpriteFrame& fram = m_data->frames[m_frameSelected];
	params.filename = m_data->spritesheetFilenames[fram.filename].c_str();
	params.x  = fram.x;
	params.y  = fram.y;
	params.w  = fram.w;
	params.h  = fram.h;
	params.cx = fram.cx;
	params.cy = fram.cy;
}

int SpriteStatic::getFrameID(std::string name)
{
	if (m_data == NULL) return -1;
	std::map<std::string, int>::iterator it = m_data->frameNames.find(name);
	if (it == m_data->frameNames.end()) return -1;
	return (int)it->second;
}

bool SpriteStatic::selectFrame(std::string name)
{
	int id = getFrameID(name);
	if (id < 0) return false;
	return selectFrame(id);
}

bool SpriteStatic::selectFrame(int frameID)
{
	if (m_data == NULL) return false;
	if (m_data->Empty()) return false;
	m_frameSelected  = frameID;
	return true;
}

bool SpriteLibrary::load(const char* filename)
{
	std::ifstream in(filename);

	if (in.fail())
	{
		dbgPrintLog("Error: %s doesn't exist.\n", filename);
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
			dbgPrintLog("Error %d: spritesheet filename undefined yet.\n", lineNum);
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
		dbgPrintLog("Error: Cannot open for save filename %s\n", filename);
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
		dbgPrintLog("Error %d: SPRITESHEET filename not found. Skipping line.\n", lineNum);
		return false;
	}
	quoteStart++;

	quoteEnd = line.find('\"', quoteStart);
	if (quoteEnd == std::string::npos) {
		dbgPrintLog("Error %d: SPRITESHEET filename incomplete. Skipping line.\n", lineNum);
		return false;
	}

	std::string spritesheetFilename = line.substr(quoteStart, quoteEnd - quoteStart);
	std::map<std::string, int>::iterator it = spritesheetIndex.find(spritesheetFilename);
	if (it == spritesheetIndex.end())
	{
		currentSpritesheet = (int) spritesheetFilenames.size();
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
		int begin = (int) sline.tellg();

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
				dbgPrintLog("Error %d: FRAME incorrect or incomplete. Skipping line.", lineNum);
				return false;
			}
		}

		//Leemos el nombre del sprite
		size_t quoteStart, quoteEnd = int(sline.tellg())-begin;

		quoteStart = line.find('\"', quoteEnd);
		if (quoteStart == std::string::npos) {
			dbgPrintLog("Error %d: FRAME name not found. Skipping line.\n", lineNum);
			return false;
		}
		quoteStart++;

		quoteEnd = line.find('\"', quoteStart);
		if (quoteEnd == std::string::npos) {
			dbgPrintLog("Error %d: FRAME name incomplete. Skipping line.\n", lineNum);
			return false;
		}

		std::string framename = line.substr(quoteStart, quoteEnd - quoteStart);
		std::map<std::string, int>::iterator it = frameNames.find(framename);
		if ( it != frameNames.end() ) //EXISTS
		{
			dbgPrintLog("Warning %d: FRAME name redefined. Overriding attributes.\n", lineNum);
			frames[it->second] = frame;
		}
		else
		{
			frames.push_back(frame);
			frameNames[framename] = (int) frames.size()-1;
		}

		return true;
}

bool SpriteLibrary::Empty()
{
	return frames.size() == 0;
}
