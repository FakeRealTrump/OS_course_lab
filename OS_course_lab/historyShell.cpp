#include "historyShell.h"

using namespace std;
historyShell::historyShell() {}


historyShell::~historyShell() {}

void historyShell::handleCommand()
{
	{
		string inputLine;
		while (1)
		{
			cout << "osh>";
			getline(cin, inputLine);
			cout << "executing: " << inputLine << endl;
			//if input is  start as '!'
			if (inputLine[0] == '!')
			{
				if (inputLine[1] == '!') 
				{
					HL.ExecuteLasteatCommand();
				}
				else
				{
					int rindex;
					if (sscanf(inputLine.c_str(), "%*c%d", &rindex) > 0)
					{
						HL.ExecuteNthCommand(rindex);
					}
					else
					{
						perror("invalid command");
					}
				}
				continue;
			}
			
			//if input is "history/History"
			if (inputLine == "history" || inputLine == "History")
			{
				HL.PrintHistory(); 
				continue;
			}
			if (inputLine == "q" || inputLine == "quit")
			{
				break;
			}
			HL.Recievecommand(inputLine);
		}
	}
	return;
}
