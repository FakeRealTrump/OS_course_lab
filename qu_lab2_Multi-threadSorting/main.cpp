#include<iostream>
#include <cstdio>
#include<vector>
#include<thread>
#include<algorithm>

using namespace std;

vector<int> unsorted;
vector<int> sorted;

void part_sort_function(vector<int>::iterator Begin, vector<int>::iterator End)
{
	//sort the element between Begin and End
	//write result into vector unsorted
	sort(Begin, End);
}
void merge_sort_function()
{
	sorted = vector<int>();
	thread sort_1(part_sort_function, unsorted.begin(), unsorted.begin() + unsorted.size() / 2);
	thread sort_2(part_sort_function, unsorted.begin() + unsorted.size() / 2 , unsorted.end());
	sort_1.join();
	sort_2.join();
	//the merging thread will be blocked untill sort_1 & sort_2 exited
	//merge vecotr_1 and vector_2
	//write result into vector sorted
	cout << "sorting finished" << endl;
	auto it_1 = unsorted.begin();
	auto it_2 = unsorted.begin() + unsorted.size() / 2;
	while (1)
	{
		if (it_1 < unsorted.begin() + unsorted.size() / 2)
		{
			if (it_2 < unsorted.end())
			{
				if (*it_1 <= *it_2)
				{
					sorted.push_back(*it_1);
					it_1++;
				}
				else
				{
					sorted.push_back(*it_2);
					it_2++;
				}
			}
			else
			{
				for (; it_1 < unsorted.begin() + unsorted.size() / 2; it_1++)
				{
					sorted.push_back(*it_1);
				}
				break;
			}
		}
		else
		{
			for (; it_2 < unsorted.end(); it_2++)
			{
				sorted.push_back(*it_2);
			}
			break;
		}
	}

	return;
}


int main(int argc, char* argv[])
{
	if (argc > 1)
	{
		unsorted = vector<int>();
		for (int i = 1; i < argc; i++)
		{
			unsorted.push_back(atoi(argv[i]));
		}
	}
	else
	{
		unsorted = vector<int>({ 7,12,19,3,18,4,2,6,15,8 });
	}
	cout << "unsorted:";
	for (auto it : unsorted)
	{
		cout << it << " ";
	}
	cout << endl;
	thread merge(merge_sort_function);
	merge.join();

	cout << "sorted:";
	for (auto it : sorted)
	{
		cout << it << " ";
	}
	cout << endl;

	return 0;
}