#include "log.h"

#include <fstream>
#include <cstdlib>

#define LOG_FILE "log.txt"

struct LogStream
{
	std::ostream *stream;

	LogStream() : stream(NULL)
	{
		std::fstream *file = new std::fstream(LOG_FILE, std::ios::out|std::ios::trunc);
		if (!file->is_open())
		{
			delete file;
			stream = &std::cerr;
			(*stream) << "Log cannot open " << LOG_FILE << ", using stderr." << std::endl;
		}
		else
		{
			stream = file;
		}
	}

	~LogStream()
	{
		if (stream == NULL) return;
		std::fstream *file = dynamic_cast<std::fstream *>(stream);
		if (file == NULL) return;
		file->close();
	}
};

LogStream output_log;
std::ostream & Log()
{
	return *(output_log.stream);
}


