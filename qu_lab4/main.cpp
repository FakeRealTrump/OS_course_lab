#include <cstdio>
#include<string>
#include<vector>
#include"busyBlock.h"
#include"freeBlock.h"
#include "main.h"
using namespace std;



int main()
{
	//接受输入启动,初始化
	int totalsize = 500;
	initMem(totalsize);
	//
	RunAsShell();
	return 0;
}