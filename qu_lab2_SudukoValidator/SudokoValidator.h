#pragma once
#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<thread>
#include<future>
#include"Grid.h"

using namespace std;
class SudokoValidator
{
private:
	Grid data;
	promise<bool> promised_result1;
	promise<bool> promised_result2;
	promise<bool> promised_result3;
	ifstream readfrom;
	SudokoValidator();
	bool CheckLine(vector<int> Line);
	bool CheckSubGrid(Grid subGrid);
	void MulThread_CheckCol();
	void MulThread_CheckRow();
	void MulThread_ChecksubGrid();
public:
	SudokoValidator(string filename);
	~SudokoValidator();
	bool Validate();
	bool MulThread_Validate();
};

