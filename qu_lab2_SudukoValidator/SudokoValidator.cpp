#include "SudokoValidator.h"
#include<sstream>
#include<regex>

using namespace std;


SudokoValidator::SudokoValidator()
{
}

bool SudokoValidator::CheckLine(vector<int> Line)
{
	bool result = true;
	sort(Line.begin(), Line.end());
	for (int i = 0; i < 9; i++)
	{
		if (Line[i] != i+1) 
		{
			result = false;
			break;
		}		
	}
	return result;
}

bool SudokoValidator::CheckSubGrid(Grid subGrid)
{
	vector<int> temp = subGrid.getData();
	return CheckLine(temp);
}

void SudokoValidator::MulThread_CheckCol()
{
	vector<bool> results;
	bool isValidate = true;
	for (int i = 0; i < 9; i++)
	{
		results.push_back(CheckLine(data.getCol(i + 1)));
	}
	for (int i = 0; i < 9; i++)
	{
		if (results[i] == false)
		{
			cout << "Col:" << i + 1 << "is not passed" << endl;
			isValidate = false;
		}
	}
	promised_result1.set_value(isValidate);
}

void SudokoValidator::MulThread_CheckRow()
{
	vector<bool> results;
	bool isValidate = true;
	for (int i = 0; i < 9; i++)
	{
		results.push_back(CheckLine(data.getRow(i + 1)));
	}
	for (int i = 0; i < 9; i++)
	{
		if (results[i] == false)
		{
			cout << "Row:" << i + 1 << "is not passed" << endl;
			isValidate = false;
		}
	}
	promised_result2.set_value(isValidate);
}

void SudokoValidator::MulThread_ChecksubGrid()
{
	/*
	vector<promise<bool>> promised_result;
	vector<future<bool>> results;
	for (int i = 0; i < 9; i++)
	{
		promise<bool> Promised;
		results.push_back(Promised.get_future());
	}
	*/
	vector<bool> results;
	bool isValidate = true;
	for (int i = 0; i < 9; i += 3)
	{
		for (int j = 0; j < 9; j += 3)
		{
			results.push_back(CheckSubGrid(data.subGrid(i + 1, i + 3, j + 1, j + 3)));
		}
	}

	for (int i = 0; i < 9; i++)
	{
		if (results[i] == false)
		{
			cout << "Grid:" << i + 1 << "is not passed" << endl;
			isValidate = false;
		}
	}
	promised_result3.set_value(isValidate);
}


//warning: you have to put the file in the same folder as the .out in Linux
SudokoValidator::SudokoValidator(string filename)
{
	//read the whole file context in a string
	readfrom.open(filename);
	stringstream buffer;

	buffer << readfrom.rdbuf();
	string contents(buffer.str());
	//cout << contents;

	//there is not a simple way to transfer a string to a vector<int>
	//so i use a regex to match all number in the whole File Context
	//then transfer the string[] to int[]
	regex expression("(\\d)+");
	regex_iterator<string::iterator> it(contents.begin(), contents.end(), expression);
	regex_iterator<string::iterator> end;


	vector<int> temp;
	for (; it != end; ++it)
	{
		temp.push_back(atoi(it->str().c_str()));
	}

	data = Grid(temp, 9, 9);
}

SudokoValidator::~SudokoValidator()
{
}

bool SudokoValidator::Validate()
{
	//single-thread:
	bool isValidate = true;
	vector<bool> results(9,true);
	for (int i = 0; i < 9; i++)
	{
		results[i] = CheckLine(data.getCol(i + 1));
	}

	for (int i = 0; i < 9; i++)
	{
		if (results[i] == false)
		{
			cout << "Col:" << i + 1 << "is not passed" << endl;
			isValidate = false;
		}
	}

	for (int i = 0; i < 9; i++)
	{
		results[i] = CheckLine(data.getRow(i + 1));
	}
	for (int i = 0; i < 9; i++)
	{
		if (results[i] == false)
		{
			cout << "Row:" << i + 1 << "is not passed" << endl;
			isValidate = false;
		}
	}

	for (int i = 0; i < 9; i+=3)
	{
		for (int j = 0; j < 9; j+=3)
		{
			results[i] = CheckSubGrid(data.subGrid(i + 1, i + 3, j + 1, j + 3));
		}
	}
	for (int i = 0; i < 9; i++)
	{
		if (results[i] == false)
		{
			cout << "Grid:" << i + 1 << "is not passed" << endl;
			isValidate = false;
		}
	}

	if (isValidate)
		cout << "pass!" << endl;
	return isValidate;
}

bool SudokoValidator::MulThread_Validate()
{
	thread t1 = thread(bind(&SudokoValidator::MulThread_CheckCol, this));
	thread t2 = thread(bind(&SudokoValidator::MulThread_CheckRow, this));
	thread t3 = thread(bind(&SudokoValidator::MulThread_ChecksubGrid, this));
	t1.detach();
	t2.detach();
	t3.detach();

	bool results[3];
	results[0] = promised_result1.get_future().get();
	results[1] = promised_result2.get_future().get();
	results[2] = promised_result3.get_future().get();
/*
	cout << "Col checks:" << results[0] << endl;
	cout << "Row checks:" << results[1] << endl;
	cout << "Grid checks:" << results[2] << endl;
*/
	if (results[0] && results[1] && results[2])
		cout << "pass!" << endl;

	return results[0] && results[1] && results[2];

}
