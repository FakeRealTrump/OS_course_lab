#pragma once
#include<iostream>
#include<string>
#include<fstream>
class FileExamine
{
public:
	FileExamine() = default;
	FileExamine(std::string FilePath);
	virtual ~FileExamine() = default;
	virtual void examword(std::string Word) = 0;
protected:
	std::ifstream readfrom;
};

FileExamine::FileExamine(std::string FilePath)
{
	readfrom.open(FilePath);
	

	if (!readfrom.is_open())
	{
		std::cout << "Error opening " << FilePath << " for input" << std::endl;
	}
	else
	{
		std::cout << "open:" << FilePath << std::endl;
	}
}




