#pragma once
//the header will only included in main.cpp, so no rules for namespace
#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include <limits>
#include"busyBlock.h"
#include"freeBlock.h"

using namespace std;
vector<freeBlock> freeMem;
vector<busyBlock> busyMem;
int totalMem;


void initMem(int totalSize)
{
	freeMem.clear();
	busyMem.clear();
	freeMem.push_back(freeBlock(0, totalSize));
	totalMem = totalSize;
}

void divideBlock(int blockindex, int needSize)
{
	int nextBlockStartAddr = freeMem.at(blockindex).start_addr + needSize;
	int nextBlockSize = freeMem.at(blockindex).size - needSize;
	freeMem.at(blockindex).size = needSize;

	freeMem.insert(freeMem.begin() + blockindex + 1, freeBlock(nextBlockStartAddr, nextBlockSize));
}

void Compress()
{
	//遍历PCB表，修改其初始地址，使其成为一片从0x0开始的连续内存区域,
	//并把空闲内存表中清空，置入一个新的，始地址为最后一个进程末尾地址，
	//大小为(全部内存-进程占用内存量)的大内存块。

	int newstartaddr = 0;
	for (auto it = busyMem.begin(); it < busyMem.end(); it++)
	{
		(*it).start_addr = newstartaddr;
		newstartaddr += (*it).size;
	}

	freeMem.clear();
	freeMem.push_back(freeBlock(newstartaddr, totalMem - newstartaddr));
}



void giveABlockToProcess(string  processName, int freeBlockIndex)
{
	//the sort is to make  busyMem sort by start_addr, not sorted by sequence of requesting memory

	busyMem.push_back(busyBlock(freeMem.at(freeBlockIndex), processName));
	sort(busyMem.begin(), busyMem.end());

	freeMem.erase(freeMem.begin() + freeBlockIndex);
}

//w: worst fit，像这样的函数有3个
//如果成功找到一个可以分割的块，这个函数分割那个块，返回想要的那个index
//假如没有块可用，返回-1
int getABlockByW(int needSize)
{
	int blockindex = -1;
	int MaxSize = needSize;

	for (auto it = freeMem.begin(); it < freeMem.end(); it++)
	{
		if ((*it).size >= MaxSize)
		{
			MaxSize = (*it).size;
			blockindex = it - freeMem.begin();
		}
	}

	if (blockindex != -1)
	{
		divideBlock(blockindex, needSize);
	}

	return blockindex;
}

int getABlockByB(int needSize)
{
	int blockindex = -1;
	int fragmentSize = (numeric_limits<int>::max)();

	for (auto it = freeMem.begin(); it < freeMem.end(); it++)
	{
		if ((*it).size >= needSize)
		{
			if ((*it).size - needSize < fragmentSize)
			{
				fragmentSize = (*it).size - needSize ;
				blockindex = it - freeMem.begin();
			}
		}
	}

	if (blockindex != -1)
	{
		divideBlock(blockindex, needSize);
	}

	return blockindex;
}

int getABlockByF(int needSize)
{
	int blockindex = -1;
	

	for (auto it = freeMem.begin(); it < freeMem.end(); it++)
	{
		if ((*it).size >= needSize)
		{			
			blockindex = it - freeMem.begin();
break;
		}
	}

	if (blockindex != -1)
	{
		divideBlock(blockindex, needSize);
	}

	return blockindex;
}



int allocateByCommand(string processname, int needSize, char waytofit, bool tryagain = true)
{
	//按参数选择算法分配
	int getBlock = -1;
	if (waytofit == 'W')
	{
		getBlock = getABlockByW(needSize);
	}
	if (waytofit == 'B')
	{
		getBlock = getABlockByB(needSize);
	}
	if (waytofit == 'F')
	{
		getBlock = getABlockByF(needSize);
	}
	//通过返回值检查是否分配成功

	if (getBlock != -1)
	{
		giveABlockToProcess(processname, getBlock);
		cout << "allocate success" << endl;
	}
	else
	{
		if (tryagain)
		{
			//先试图紧缩
			cout << "no enough memory, we'll try compress first" << endl;
			Compress();
			allocateByCommand(processname, needSize, waytofit, false);
		}
		else
		{
			cout << "not enough memory, failed" << endl;
		}
	}

}


void release(string processName)
{
	//遍历查找得process index
	int index = -1;
	for (auto it = busyMem.begin(); it < busyMem.end(); it++)
	{
		if ((*it).name == processName)
		{
			index = it - busyMem.begin();
			break;
		}
	}
	if (index == -1)
	{
		cout << "process not found, maybe allocated fail before or had been freed" << endl;
		return;
	}
	busyBlock needTofree = busyMem.at(index);
	//通过比较开始地址找插入位置
	auto  insertPos = freeMem.begin();
	for (auto it = freeMem.begin(); it < freeMem.end(); it++)
	{
		if ((*it).start_addr >= needTofree.start_addr)
		{
			insertPos = it;
			break;
		}
	}

	bool hadbeenrelocated = false;
	//前面的free块与其连接
	if (insertPos != freeMem.begin())
	{
		if ((*(insertPos - 1)).start_addr + (*(insertPos - 1)).size == needTofree.start_addr)
		{
			(*(insertPos - 1)).size += needTofree.size;
			hadbeenrelocated = true;
		}
	}
	//
	if (insertPos != freeMem.end()-1&& !hadbeenrelocated)
	{
		if (needTofree.start_addr + needTofree.size == (*(insertPos)).start_addr)
		{
			(*(insertPos)).start_addr -= needTofree.size;
			(*(insertPos)).size += needTofree.size;
			hadbeenrelocated = true;
		}
	}

	if (!hadbeenrelocated)
	{
		freeMem.insert(insertPos, freeBlock(needTofree.start_addr, needTofree.size));
	}

	busyMem.erase(busyMem.begin() + index);
	return;
}

void stat()
{
	auto it1 = freeMem.begin();
	auto it2 = busyMem.begin();
	
	if (freeMem.empty())
	{
		
		while (it2 != busyMem.end())
		{			
			cout << (*it2);
			it2++;
		}
		return;
	}

	if (busyMem.empty())
	{
		while (it1 != freeMem.end())
		{		
			cout << (*it1);
			it1++;
		}
		
		return;
	}

	
	while (it1 != freeMem.end() &&  it2 != busyMem.end())
	{
		if ((*it1) > (*it2) )
		{
			
			cout << (*it2);
			it2++;
		}
		else
		{
		
			cout << (*it1);
			it1++;
		}
	}
	
	//有一个队列被输出完毕,遍历输出为空队列就行了
	while (it2 != busyMem.end())
	{
		//cout << (*it2).start_addr << ": " << (*it2).start_addr + (*it2).size << " process" << (*it2).name << endl;
		cout << (*it2);
		it2++;
	}
	
	while (it1 != freeMem.end())
	{
		cout << (*it1);
		it1++;
	}
}

void RunAsShell()
{
		string inputLine;
		char temp[255];
		while (1)
		{
			cout << "allocator>";
			getline(cin, inputLine);
			if (inputLine.at(0) == 'R'&& inputLine.at(1) == 'Q')
			{
				//拆分得到参数
				string processName;
				int needSize;
				sscanf(inputLine.c_str(), "%*s%s%d%*s", temp, &needSize);
				processName = string(temp);

				int tail = inputLine.size() - 1;
				while (inputLine.at(tail) == ' ' || inputLine.at(tail) == '\n' || inputLine.at(tail) == '\r')
					tail--;
				char waytofit = inputLine.at(tail);

				allocateByCommand(processName, needSize, waytofit, true);
			}
			if (inputLine.at(0) == 'R'&& inputLine.at(1) == 'L')
			{
				string processName;
				sscanf(inputLine.c_str(), "%*s%s", temp);
				processName = string(temp);
				release(processName);
			}
			if (inputLine.at(0) == 'C')
			{
				Compress();
				stat();
			}

			if (inputLine.at(0) == 'X')
			{
				return;
			}
			if (inputLine == "STAT")
			{
				stat();
			}
		}
		
	return;
}
