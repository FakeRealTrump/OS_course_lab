#include <cstdio>
#include<vector>
#include"FileExamine_pipe_impl.h"

using namespace std;
int main(int argc, char* argv[])
{
	if (argc < 3) {
		printf("hello from qu_lab3!\n");
		FileExamine_pipe_impl test("ANNA_KARENINA.txt");
		test.examword("she");
		return 0;
	}
	else
	{
		string filepath(argv[1]);
		vector<string> words;
		for (int i = 2; i < argc; i++)
		{
			words.push_back(string(argv[i]));
		}

		FileExamine_pipe_impl test(filepath);
		for (auto word : words)
		{
			test.examword(word);
		}
	}
	return 0;
}