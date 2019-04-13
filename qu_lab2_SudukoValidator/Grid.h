#pragma once
#include<vector>
//if the argument is wrond, the method of Grid would throw a exception
class Grid
{
	int cols;
	int rows;
	std::vector<int> data;	
public:
	Grid();
	Grid(std::vector<int>data, int cols, int rows);
	Grid subGrid(int start_cols, int end_cols, int start_rows, int end_rows);
	std::vector<int> getCol(int col_index);
	std::vector<int> getRow(int row_index);
	std::vector<int> getData();
	~Grid();
};

