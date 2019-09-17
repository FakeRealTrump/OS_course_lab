#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include <time.h>
#include<list>

using namespace std;

const int maxFrames = 128;

struct page {
	int size = 256;
};
struct frame {
	int size = 256;
	bool busy = false;
	char context[256];
};
struct virtualmem {
	page pages[256];
};
struct physicalmem {
	frame frames[maxFrames];
};

struct pageTableItem {
	int pageNum;
	int frameNum;
	bool avaliable;
};

struct pageTable {
	pageTableItem table[256];
};

struct TLB {
	list<pageTableItem> table;
	int maxsize = 16;
};

virtualmem vm;
physicalmem pm;
pageTable pt;
TLB tlb;
ifstream disk;

void init()
{
	for (int i = 0; i < 256; i++)
	{
		pt.table[i] = { i,-1,false };
	}
	disk.open("C:\\Users\\CROW\\source\\repos\\lab5\\lab5\\BACKING_STORE.bin", ios::binary);
	if (!disk)
	{
		cerr << "backstore file open failed!" << endl;
	}
}

int getAframe()
{
	for (int i = 0; i < maxFrames; i++)
	{
		if (pm.frames[i].busy == false)
		{
			return i;
		}
	}

	srand(time(NULL));
	return rand() / maxFrames;
}

frame ReadFromDisk(int pageNum)
{

	char diskcontext[256];
	disk.seekg(pageNum * 256, ios::beg);
	disk.read(diskcontext, 256);
	frame newframe = { 256,true };
	memcpy_s(newframe.context, 256, diskcontext, 256);
	return newframe;
}

void removeAFrame(int frameNum)
{
	for (int i = 0; i < 256; i++)
	{
		if (pt.table[i].frameNum == frameNum)
		{
			pt.table[i].avaliable = false;
		}
	}

	for (auto i : tlb.table)
	{
		if (i.frameNum == frameNum)
		{
			i.avaliable = false;
		}
	}

}

int searchInTLB(int pageNum)
{
	for (auto item :tlb.table)
	{
		if (item.pageNum == pageNum && item.avaliable)
		{
			return item.frameNum;
		}
	}
	return -1;
}

void insertOrRepalaceATLBItemByFIFO(pageTableItem newItem)
{
	if (tlb.table.size() < tlb.maxsize)
	{
		tlb.table.push_back(newItem);
	}
	else
	{
		tlb.table.pop_front();
		tlb.table.push_back(newItem);
	}
}

int main(int argc, char* argv[])
{

	init();
	ifstream fin;
	if (argc > 1)
	{
		fin.open(argv[1]);
	}
	else
	{
		fin.open("addresses.txt");
	}
	int tlbmiss = 0;
	int pageFaultTime = 0;
	int visitMemoryTime = 0;

	int virtualAddr = 0;
	int pageNum;
	int frameNum;
	int offset;
	int physicAddr;
	int theByte;
	frame newFrame;

	while (fin >> virtualAddr)
	{
		visitMemoryTime++;
		pageNum = virtualAddr & 65280;
		pageNum = pageNum >> 8;
		offset = virtualAddr & 255;

		frameNum = searchInTLB(pageNum);
		if (frameNum != -1) 
		{
			tlbmiss++;
			theByte = pm.frames[frameNum].context[offset];
			cout << "  Virtual address :  " << virtualAddr << "  Physical address :  " << frameNum * 256 + offset \
				<< "  Value :  " << theByte << endl;
		}
		else
		{
			if (pt.table[pageNum].avaliable)
			{
				frameNum = pt.table[pageNum].frameNum;
				theByte = pm.frames[frameNum].context[offset];
				cout << "  Virtual address :  " << virtualAddr << "  Physical address :  " << frameNum * 256 + offset \
					<< "  Value :  " << theByte << endl;
			}
			else
			{
				pageFaultTime++;
				frameNum = getAframe();
				newFrame = ReadFromDisk(pageNum);
				removeAFrame(frameNum);
				pm.frames[frameNum] = newFrame;

				pt.table[pageNum].frameNum = frameNum;
				pt.table[pageNum].avaliable = true;

				insertOrRepalaceATLBItemByFIFO({ pageNum,frameNum,true });
				theByte = pm.frames[frameNum].context[offset];
				cout << " Virtual address : " << virtualAddr << " Physical address : " << frameNum * 256 + offset \
					<< "Value : " << theByte << endl;;
			}
		}
	}

	cout << " Total visit memory Time : " << visitMemoryTime << " TLB hit time  " \
		<< visitMemoryTime - tlbmiss\
		<< "  pageFault occured:  " << pageFaultTime << endl;

	double tlbhitrate = (double)(visitMemoryTime - tlbmiss) / visitMemoryTime;
	double pagefaultrate = (double)(pageFaultTime) / visitMemoryTime;

	cout << " TLB hit rate: " << tlbhitrate \
		<< " pageFault rate:  " << pagefaultrate<< endl;
}