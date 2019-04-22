#pragma once
#include<iostream>
#include<string>
#include<fstream>
#include<regex>

namespace FileEx {
	using namespace std;
	class FileExamine
	{
	public:
		FileExamine() = default;
		FileExamine(string FilePath);
		virtual ~FileExamine() = default;
		virtual void examword(string Word) {};
		static bool checkWord(string Sentence, string Word);
		void readyForNextExam();
	protected:
		ifstream readfrom;
	};

	inline FileExamine::FileExamine(string FilePath)
	{
		readfrom.open(FilePath);

		if (!readfrom.is_open())
		{
			cout << "Error opening " << FilePath << " for input" << endl;
		}
		else
		{
			cout << "open:" << FilePath << endl;
		}
	}

	inline bool FileExamine::checkWord(string Sentence, string Word)
	{
		string expression("[^0-9a-zA-Z](" + Word + ")[^0-9a-zA-Z]");
		regex pattern(expression, regex::icase);
		if (regex_search(Sentence, pattern))
		{
			return true;
		}
		return false;
	}

	inline void FileExamine::readyForNextExam()
	{
		readfrom.clear();
		readfrom.seekg(0, ios::beg);
	}

}


