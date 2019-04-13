#include "Grid.h"
using namespace std;

Grid::Grid()
{
}

Grid::Grid(std::vector<int> data, int cols, int rows)
{
	//check if the argument is reasonable 
	if (data.size() == cols * rows)
	{
		this->data = data;
		this->cols = cols;
		this->rows = rows;
	}
	else 
	{
		//to do: throw an expection data size is not right
		throw(1);
	}
}

Grid Grid::subGrid(int start_cols, int end_cols, int start_rows, int end_rows)
{
	if (0 < start_cols&&end_cols <= cols && 0 < start_rows&&end_cols <= rows)
	{
		vector<int> temp;
		for (int i = start_cols - 1; i < end_cols; i++)
		{
			for (int j = start_rows - 1; j < end_rows; j++)
			{
				temp.push_back(data[i*cols + j]);
			}
		}
		return Grid(temp, (end_cols - start_cols + 1), (end_rows - start_rows + 1));
	}
	else
	{

	}
}



std::vector<int> Grid::getRow(int row_index)
{
	if (row_index <= rows)
	{
		//data[row_index*cols - cols, row_index*cols - cols)
		return vector<int>(data.begin() + (row_index - 1)*cols, data.begin() + row_index * cols);
	}
	else
	{

	}
}

std::vector<int> Grid::getData()
{
	return vector<int>(data);
}

std::vector<int> Grid::getCol(int col_index)
{
	if (col_index <= cols)
	{
		vector<int> temp;
		for (auto it = data.begin()+col_index-1;it<data.end();it+=cols)
		{
			temp.push_back(*it);
		}
		return temp;
	}
	else
	{

	}
}

Grid::~Grid()
{
}
