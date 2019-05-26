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
	//����PCB���޸����ʼ��ַ��ʹ���ΪһƬ��0x0��ʼ�������ڴ�����,
	//���ѿ����ڴ������գ�����һ���µģ�ʼ��ַΪ���һ������ĩβ��ַ��
	//��СΪ(ȫ���ڴ�-����ռ���ڴ���)�Ĵ��ڴ�顣

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

//w: worst fit���������ĺ�����3��
//����ɹ��ҵ�һ�����Էָ�Ŀ飬��������ָ��Ǹ��飬������Ҫ���Ǹ�index
//����û�п���ã�����-1
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
	//������ѡ���㷨����
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
	//ͨ������ֵ����Ƿ����ɹ�

	if (getBlock != -1)
	{
		giveABlockToProcess(processname, getBlock);
		cout << "allocate success" << endl;
	}
	else
	{
		if (tryagain)
		{
			//����ͼ����
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
	//�������ҵ�process index
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
	//ͨ���ȽϿ�ʼ��ַ�Ҳ���λ��
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
	//ǰ���free����������
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
	
	//��һ�����б�������,�������Ϊ�ն��о�����
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
				//��ֵõ�����
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
