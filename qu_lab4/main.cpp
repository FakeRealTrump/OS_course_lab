#include <cstdio>
#include<string>
#include<vector>
#include"busyBlock.h"
#include"freeBlock.h"
#include "main.h"
using namespace std;



int main(int argc, char* argv[])
{
	//������������,��ʼ��
	int totalsize = 1000;
	
	if (argc > 1)
	{
		totalsize = atoi(argv[1]);
	}

	initMem(totalsize);
	cout << "Init:" << totalMem << "B" << endl;
	//
	RunAsShell();
	return 0;
}