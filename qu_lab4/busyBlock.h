#pragma once
#include "Block.h"
#include "freeBlock.h"
#include<string>

class busyBlock: public Block {
public:
	std::string name = "";

	busyBlock(){}
	busyBlock(int start_addr, int size, std::string name): Block(start_addr, size)
	{
		this->name = name;
	}

	busyBlock(freeBlock givenBlock, std::string name): Block(givenBlock)
	{
		this->name = name;
	}
	friend std::ostream & operator<<(std::ostream &out, const busyBlock &block);
};

std::ostream & operator<<(std::ostream &out, const busyBlock &block)
{
	return out << "Addresses[" << block.start_addr << ":" << block.start_addr + block.size << "] Process " <<block.name <<std::endl;
}