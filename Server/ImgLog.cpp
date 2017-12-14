#include <fstream>
#include "ImgLog.hpp"

ImgLog::ImgLog()
{
}

ImgLog::~ImgLog()
{
}

void	ImgLog::getData(std::string path)const
{
	std::string fileName;

	fileName = path + _name;
	std::ofstream file(fileName.c_str(), std::ios::out | std::ios::binary | std::ios::app);

	if (file.is_open())
	{
		for (auto & it : _data)
			file.write(&it, 1);
		file.close();
	}
}

void	ImgLog::setData(const std::string inputFile, const std::string filename)
{
	std::ifstream	file(inputFile.c_str(), std::ios::in | std::ios::binary);
	std::ostringstream      tmp;

	if (file.is_open())
	{
		tmp << file.rdbuf();
		for (auto & it : tmp.str())
			_data.push_back(it);
	}
	_name = filename;
}