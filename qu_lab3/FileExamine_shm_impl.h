#pragma once
#include "FileExamine.h"
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include<stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>


const int BufferSize = 4096;
namespace FileEx {

	class FileExamine_shm_impl :
		public FileExamine
	{
	public:
		FileExamine_shm_impl() = default;
		~FileExamine_shm_impl() = default;
		FileExamine_shm_impl(std::string FilePath) :FileExamine(FilePath) {}
		void examword(std::string Word) override;
		void mapper(int start, int end) {}
		void reducer() {}
	};

	void FileExamine_shm_impl::examword(std::string Word)
	{
		string path = "//tmp//os_lab3";
		printf("checking for Word: %s\n", Word.c_str());
		fflush(stdout);

		pid_t pid = fork();
		if (pid == 0)
		{//child
			int fd = shmget(0x2234, BufferSize, IPC_CREAT | 0666);
			char* p = (char*)shmat(fd, NULL, 0);
			
			close(fd);
		//	sleep(10);
			cout << "process:" << getpid() << " recv:" << p << endl;
		}
		else if (pid > 0)
		{
			//father
			// read some context from the ifstream, write it to the shm
			//setup a lock or something so the child could konw when to read from shm
			char txtBuffer[1024];
			while (!readfrom.eof())
			{
				readfrom.read(txtBuffer, sizeof(txtBuffer));
				//write it to shm and
				int fd = shmget(0x2234, BufferSize, IPC_CREAT | 0666);
				char* p = (char*)shmat(fd, NULL, 0);
				memmove(p, txtBuffer, sizeof(txtBuffer));
				cout << "process:" << getpid() << " send:" << sizeof(txtBuffer) << endl;

			}
		}
		readyForNextExam();
	}

}
