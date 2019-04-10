#pragma once
#include"command.h"
#include"cycleList.h"
#include<iostream>
#include<fstream>
class historyList {
private:
	int commandIndex;
	cycleList<command> list;
	std::string historyFileName = "history.txt";
	std::fstream historyFile;

	void RecordAndExecute(std::string Fullcommand);
	void Record(std::string Fullcommand);
	void Execute(command Command);
	bool IsCommandAvailiable(command Command);

	std::string EncrypyCommandWithIndex(std::string CommandWithIndex);
	std::string MakeCommandWithIndex(std::string OriginCommand);
	
public:
	historyList();
	~historyList();
	void ReadHistory();
	
	void Sethistoryfile(std::string NewFileName);
	void Recievecommand(std::string Fullcommand);
	void ExecuteLasteatCommand();
	void ExecuteNthCommand(int N);
	void PrintHistory();
	int historySize();
	static void test();
};