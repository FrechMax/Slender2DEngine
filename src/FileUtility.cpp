#include "FileUtility.h"

#include <fstream>

using namespace Utility;

IOFile::IOFile(std::string filename)
{
	mFilename = filename;
}

void IOFile::open(int mode)
{
	int modeFlags = 0;
	if ((mode & Mode::MODE_APPEND) > 0) 
	{
		modeFlags |= std::ios::app;
	} 
	if ((mode & Mode::MODE_OVERWRITE) > 0)
	{
		modeFlags |= std::ios::trunc;

	}
	if ((mode & Mode::MODE_BINARY) > 0)
	{
		modeFlags |= std::ios::binary;
	}
	mIOStream.open(mFilename.c_str(), modeFlags);
}

bool Utility::IOFile::isOpen()
{
	return mIOStream.is_open();
}



void IOFile::close()
{
	mIOStream.close();
}

int IOFile::readFromFile(char * destination, int count, const char * sourcePath)
{
	std::ifstream ifs;
	ifs.open(sourcePath, std::ios::binary);

	if (ifs.is_open())
	{
		ifs.read(destination, count);

		ifs.close();
	}

	return 0;
}

int IOFile::writeToFileB_OW(const char * source, int count, const char * destinationPath)
{
	std::ofstream of;
	of.open(destinationPath, std::ios::binary | std::ios::trunc);

	return writeToStreamB(of, source, count);
}

int IOFile::writeToFileB_APP(const char * source, int count, const char * destinationPath)
{
	std::ofstream of;
	of.open(destinationPath, std::ios::binary | std::ios::app);

	return writeToStreamB(of, source, count);
}

int IOFile::writeToStreamB(std::ofstream& of, const char * source, int count)
{
	if (of.is_open()) {
		of.write(source, count);

		of.close();
	}

	return 0;
}