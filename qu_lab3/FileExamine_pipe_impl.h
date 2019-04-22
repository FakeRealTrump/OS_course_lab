#pragma once
#include "FileExamine.h"
#include <unistd.h>
#include<cstring>
#include <sys/types.h>
#include <signal.h>
#include<regex>

namespace FileEx {
	class FileExamine_pipe_impl :
		public FileExamine
	{
	public:
		FileExamine_pipe_impl() = default;
		~FileExamine_pipe_impl() = default;
		FileExamine_pipe_impl(string FilePath) :FileExamine(FilePath) {}
		void examword(string Word) override;
	};

	void FileExamine_pipe_impl::examword(string Word)
	{

		printf("checking for Word: %s\n", Word.c_str());
		fflush(stdout);

		int fd[2];
		int ret = pipe(fd);
		if (ret == -1)
		{
			perror("Pipe Create Error");
			return;
		}
		pid_t pid = fork();
		if (pid == 0)
		{//child
			close(fd[1]);
			char mesg[200];
			string temp;
			while (1)
			{
				//read from pipe , use find() to check if the line contain our intersting word
				memset(mesg, '\0', sizeof(mesg));
				read(fd[0], mesg, sizeof(mesg));
				temp = string(mesg);
				if (checkWord(temp, Word))
				{
					cout << temp << endl;
				}
				else
				{
					//printf("nan, not found\n");
				}
				//	usleep(500);
			}
			_exit(0);
		}
		else if (pid > 0)
		{//father
			close(fd[0]);
			char* ptr = nullptr;
			string test;
			while (!readfrom.eof())
			{
				//write a line of file to child
				getline(readfrom, test);
				write(fd[1], test.c_str(), test.size() + 1);
				//	usleep(500);
			}

			//kill(pid, SIGKILL);
		}

		readyForNextExam();
	}

}