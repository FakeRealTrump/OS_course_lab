#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include <time.h>


using namespace std;
const int maxFrames = 256;
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



virtualmem vm;
physicalmem pm;
pageTable pt;
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
		cerr<< "backstore file open failed!" << endl;
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
	disk.seekg( pageNum * 256, ios::beg);
	disk.read(diskcontext, 256);
	frame newframe = { 256,true };
	memcpy_s(newframe.context, 256,diskcontext, 256);
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

	int virtualAddr = 0;
	int pageNum;
	int frameNum;
	int offset;
	int physicAddr;
	int theByte;
	frame newFrame;

	while(fin>>virtualAddr)
	{

		pageNum = virtualAddr & 65280;
		pageNum = pageNum>>8;
		offset = virtualAddr & 255;
		
		if (pt.table[pageNum].avaliable)
		{
			frameNum = pt.table[pageNum].frameNum;
			theByte = pm.frames[frameNum].context[offset];
			cout << " Virtual address : " << virtualAddr << " Physical address : " << frameNum * 256 + offset \
					<< " Value : " << theByte<<endl;
		}
		else
		{
			frameNum = getAframe();
			newFrame = ReadFromDisk(pageNum);
			removeAFrame(frameNum);
			pm.frames[frameNum] = newFrame;

			pt.table[pageNum].frameNum = frameNum;
			pt.table[pageNum].avaliable = true;

			theByte = pm.frames[frameNum].context[offset];
			cout << " Virtual address : " << virtualAddr << " Physical address : " << frameNum * 256 + offset \
				<< "Value : " << theByte << endl;;
		}
	}
}