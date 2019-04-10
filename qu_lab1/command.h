#pragma once
#include<tr1/memory>
#include<string>
#include<vector>
#include<iostream>
class command{
private:
	std::string fullcommand;
	std::string simplecommand;
	std::vector<std::string> args;
	bool checkIfNeedToWait();
	bool needtowait = true;
	char** char_args;
	char* one_char_arg;
	char* char_command;
	command() {}
	void doCleanJobs();
public:
	command(std::string Fullcommand);
	//command(char* fullcommand);
	~command();
	void execute();
	static void test();
	bool isavaliable();
	std::string getcommand();
};