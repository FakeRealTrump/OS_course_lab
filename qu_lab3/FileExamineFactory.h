#pragma once
#include"FileExamine_shm_impl.h"
#include"FileExamine_pipe_impl.h"
#include"FileExamine_socket_impl.h"

namespace FileEx {
	enum FileExamineType{PIPE,SOCKET,SHM};
	class FileExamineFactory
	{
	public:
		FileExamineFactory();
		~FileExamineFactory();
		FileExamine* create(FileExamineType Type, string FilePath)
		{
			switch (Type) {
			case PIPE:
				return new FileExamine_pipe_impl(FilePath);
				break;
			case SOCKET:
				return new FileExamine_socket_impl(FilePath);
				break;
			case SHM:
				return new FileExamine_shm_impl(FilePath);
				break;
			}
		}
	};



	FileExamineFactory::FileExamineFactory()
	{
	}


	FileExamineFactory::~FileExamineFactory()
	{
	}


}