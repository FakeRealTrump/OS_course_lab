#include"command.h"
#include<iostream>
#include<stdlib.h>
#include<string.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <wait.h>

using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::make_shared;
using std::shared_ptr;

bool isspace(char a)
{
	return a == ' ';
}

bool hasreturn(int a)
{
	return true;
}



bool command::checkIfNeedToWait()
{

	auto it = args.end() - 1;
	if ((*it) == "&") {
		args.pop_back();
		return false;
	}
	return true;
}

void command::doCleanJobs()
{
	int arg_nums = args.size();
	if (char_args != nullptr)
	{
		
		for (int i = 0; i < arg_nums; i++)
		{
			delete[] char_args[i];
			char_args[i] = nullptr;
		}
		
		delete[] char_args;
	}
	

	//one_char_arg is the last arg in char_args[]
	//just before the nullptr, so it has been freed before
	/*
	if (one_char_arg != nullptr)
		 delete[] one_char_arg;
	*/
	if (char_command != nullptr)
		delete[] char_command;

	char_args = nullptr;
	one_char_arg = nullptr;
	char_command = nullptr;
}

command::command(string Fullcommand)
{
	this->fullcommand = Fullcommand;
	//use two iterator to do a pre-search
	auto it = fullcommand.begin();
	auto pre_it = it;

	//search simple command
	while(isspace(*it)){
			it++;
		}
	pre_it = it+1;
	while(pre_it != fullcommand.end() && !isspace(*pre_it)){
			pre_it++;
		}
	simplecommand = string(it, pre_it);
	it = pre_it + 1;

	//build args
	args.push_back(simplecommand);
	while(pre_it != fullcommand.end())
	{
		while(isspace(*it)){
			it++;
		}
		pre_it = it+1;
		while(pre_it != fullcommand.end()&& !isspace(*pre_it)){
			pre_it++;
		}
		args.push_back(string(it, pre_it));
		it = pre_it + 1;
	}

	needtowait = checkIfNeedToWait();
}

//a server error
//in parent/child proc, the command will be deconstruct twice
//so the pointer will be free twice too
//if wait, the parent proc will crash
command::~command()
{
}

void command::execute()
{

	int status = -1;

	char_command = new char[simplecommand.length() + 1];
	strcpy(char_command, simplecommand.c_str());

	int arg_num = args.size();
	
	char_args = new char*[arg_num+1];
	memset(char_args, 0, sizeof(char*)*arg_num);
	
	for (int i=0;i< arg_num;i++)
	{
		one_char_arg = new char[(args[i].size() + 1)];
		memset(one_char_arg, 0, (args[i].size() + 1) * sizeof(char));
		strcpy(one_char_arg, args[i].c_str());
		char_args[i] = one_char_arg;
	}
	//the last args has to be null
	char_args[arg_num] = nullptr;
	

	int pid = fork();
	if (pid < 0) {
		perror("fork error");
		exit(0);
	}
	else if (pid > 0)
	{
		if (needtowait)
		{
			wait(nullptr);
		}
		doCleanJobs();
	}
	else if (pid == 0) {/*在子进程里*/
	   //whatever it return means exec failed
		if (hasreturn(execvp(char_command, char_args)))
		{
			perror("Fail to execute command");
		}
		_exit(0);
	}
	//doCleanJobs();
}

void command::test(){
	string a = "ls";
	string b = "rcp 218.6.132.5:./testfile testfile";
	char c[] = "dir -l /home";
	command ta(a);
	command tb(b);
	command tc(c);
	vector<command> tv;
	tv.push_back(ta);
	tv.push_back(tb);
	tv.push_back(tc);
	for(auto it:tv)
	{
		for(auto iq:it.args)
			cout<<iq<<endl;
	}
	ta.execute();
	tc.execute();
	return ;
}

//this code is from https://blog.csdn.net/tankai19880619/article/details/49678565?utm_source=blogxgwz3
//but the orgin code has two servere bug,
//1. not all path from $PATH is avaliable, you need to check it
//2. the buffer size is too small,very likely to  lead to stack smashing
bool command::isavaliable()
{

	char* char_simple_command;
	char_simple_command = new char[256];
	strcpy(char_simple_command, simplecommand.c_str());

	DIR *d;
	struct dirent *ptr;
	char temp[512];
	char *dir;
	//search the simple command in path
	//if there is not result found, the command is not avaliable
	char *path = getenv("PATH");
	strcpy(temp, path);
	dir = strtok(temp, ":");
	while (dir) {
		d = opendir(dir);
		if (d != nullptr) {
			while ((ptr = readdir(d)) != NULL)
				if (strcmp(ptr->d_name, char_simple_command) == 0) {
					closedir(d);				
					return true;
				}
			closedir(d);
		}
		dir = strtok(NULL, ":");
	}
	return false;
}

string command::getcommand()
{
	return fullcommand;
}
