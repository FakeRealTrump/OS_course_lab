#pragma once
#include<iostream>
#include"Block.h"

class freeBlock : public Block {
public:
	freeBlock() {}
	freeBlock(int start_addr, int size) :Block(start_addr, size)
	{
	}
	friend std::ostream & operator<<(std::ostream &out, const freeBlock &block);

};

std::ostream & operator<<(std::ostream &out, const freeBlock &block)
{
	return out << "Addresses[" << block.start_addr << ":" << block.start_addr + block.size << "] Unused" << std::endl;
}
