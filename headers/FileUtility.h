#pragma once

#include <fstream> 

#define FILE_OPTION_BINARY std::ios::binary
#define FILE_OPTION_APPEND std::ios::app
#define FILE_OPTION_OVERWRITE std::ios::trunc

namespace Utility {

	class IOFile
	{
	public:
		struct Mode
		{
			static const int MODE_APPEND	= 0b001;
			static const int MODE_OVERWRITE = 0b010;
			static const int MODE_BINARY	= 0b100;
		};

		void open(int mode);
		bool isOpen();
		void close();

		IOFile(std::string filename);

		int readFromFile(char* destination, int count, const char* sourcePath);

		// output 
		int writeToFileB_OW(const char* source, int count, const char* destinationPath);

		int writeToFileB_APP(const char* source, int count, const char* destinationPath);

		template <typename T>
		std::fstream& operator <<(T value) 
		{
			mIOStream << value;
		}

		template <typename T>
		std::fstream& operator >>(T value)
		{
			mIOStream >> value;
		}
	protected:
		int writeToStreamB(std::ofstream& of, const char* source, int count);

		std::fstream mIOStream;
		std::string mFilename;
	};


	template <typename T>
	int writeToFile(T source, const char* destinationPath, int foptions)
	{
		{
			std::ofstream of;
			of.open(destinationPath, std::ios::binary);

			if (of.is_open()) {
				of << source;

				of.close();
			}

			return 0;
		}
	}



}