#pragma once
#include "historyList .h"
class historyShell
{
	historyList HL;
public:
	historyShell();
	~historyShell();
	void handleCommand();
};

