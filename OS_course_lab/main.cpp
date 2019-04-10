#include"command.h"
#include"cycleList.h"
#include"historyList .h"
#include"historyShell.h"
#include<fstream>

using namespace std;

int main()
{
	//command::test();
	//cycleList<command>::test();
	historyShell shell;
	shell.handleCommand();
	return 0;
}