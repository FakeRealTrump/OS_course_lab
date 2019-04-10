#include "historyList .h"
#include<string.h>


using namespace std;

void historyList::RecordAndExecute(std::string Fullcommand)
{
	command cmd = command(Fullcommand);
	commandIndex++;	
	/*
	if (IsCommandAvailiable(cmd)) {
	}
	else {
		perror("command is unavliable");
	}
	*/
	//no matter the command is avliable or not
	//we should sent it to execvp anyway
	
	Record(Fullcommand);
	Execute(cmd);
}

void historyList::Record(std::string Fullcommand)
{
	list.push_back(command(Fullcommand));	
	string commandWithIndex = MakeCommandWithIndex(Fullcommand);

	historyFile << commandWithIndex<<std::endl;
	historyFile << std::flush;
	return;
}

void historyList::Execute(command Command)
{
	Command.execute();
}

bool historyList::IsCommandAvailiable(command Command)
{
	return Command.isavaliable();
}

void historyList::ExecuteLasteatCommand()
{
	if (list.size() > 0) 
	{
		auto cmd = list.getLastest();
		cmd.execute();
	}
	else
	{
		perror("No history command");
	}
}

void historyList::ExecuteNthCommand(int N)
{
	if (N <= list.size()&& N>0) 
	{
		auto cmd = list.get(N-1);
		cmd.execute();
	}
	else
	{
		perror("No such index history command");
	}
}

void historyList::PrintHistory()
{
	int count = list.size();
	for (int i = count; i > 0; i--)
	{
		cout << i  << ": " << list.get(i-1).getcommand() << endl;
	}
}

int historyList::historySize()
{
	return list.size();
}

std::string historyList::EncrypyCommandWithIndex(std::string CommandWithIndex)
{
	char* temp;
	temp = new char[256];
	//throw the digit index and ':'
	sscanf(CommandWithIndex.c_str(), "%*d:%[^\n]", temp);
	return string(temp);
}

std::string historyList::MakeCommandWithIndex(std::string OriginCommand)
{
	char* temp;
	temp = new char[256];
	sprintf(temp, "%d: %s", commandIndex, OriginCommand.c_str());
	return string(temp);
}

historyList::~historyList()
{
	if (historyFile.is_open())
		historyFile.close();
}

historyList::historyList()
{
	ReadHistory();
	
}

void historyList::ReadHistory()
{
	historyFile.open(historyFileName, fstream::in);
	int historycommandnum = 0;
	string commandbuffer;
	while (!historyFile.eof())
	{
		getline(historyFile, commandbuffer);
		if (commandbuffer.size() < 1) 
		{
			break;
		}
		commandbuffer = EncrypyCommandWithIndex(commandbuffer);
		historycommandnum++;
		list.push_back(command(commandbuffer));
	}
	commandIndex =  historycommandnum;

	historyFile.close();
	historyFile.clear();
	historyFile.open(historyFileName, fstream::out | fstream::app);
}

void historyList::Sethistoryfile(std::string NewFileName)
{
	historyFileName = NewFileName;
	historyFile.close();
	historyFile.open(historyFileName);
	ReadHistory();
}

void historyList::Recievecommand(string Fullcommand)
{		
		RecordAndExecute(Fullcommand);
}

void historyList::test()
{
	historyList instance;
	instance.PrintHistory();
	instance.Recievecommand("ls");
}
