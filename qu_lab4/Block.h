#pragma once

class Block {
public:
	int start_addr = 0;
	int size = 0;

	Block() {}
	Block(int start_addr, int size)
	{
		this->start_addr = start_addr;
		this->size = size;
	}
	//I'm pretty sure I wont use any thing like Block[] = {busyBlock a, freeBlock b}, so no virtual deconstructor

	//the following two operator is override , so we could  use sort() to sort  a list of busyBlock/freeBlock
	bool operator<(const Block& right)
	{
		return this->start_addr < right.start_addr;
	}

	bool operator>(const Block& right)
	{
		return this->start_addr > right.start_addr;
	}
};
