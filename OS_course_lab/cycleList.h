#pragma once
#include<list>
#include"command.h"
#include<iostream>

template<class T>
class cycleList {
private:
	std::list<T> container;
	int maxSize = 10;
public:
	cycleList() {}
	void push_back(T element);
	T get(int index);
	T getLastest();
	T getLast(int N);
	int size();
	static void test();
};

template<class T>
inline void cycleList<T>::push_back(T element)
{
	//push a element in the back
	//if the container already has maxSize element
	//remove the earlieast 
	if (container.size() < maxSize) {
		container.push_back(element);
	}
	else {
		container.pop_front();
		container.push_back(element);
	}
	return;
}

template<class T>
inline T cycleList<T>::get(int index)
{
	auto it = container.begin();
	//stl::list dont support random get
	for (int i = 0; i < index; i++)
	{
		++it;
	}
	return (*it);
}

template<class T>
inline T cycleList<T>::getLastest()
{
	int index = container.size()-1;
	return get(index);
}

template<class T>
inline T cycleList<T>::getLast(int N)
{
	int size = container.size();
	int index = size - N-1;
	return get(index);
}

template<class T>
inline int cycleList<T>::size()
{
	return container.size();
}

template<class T>
inline void cycleList<T>::test()
{
	cycleList<command> test_command;
	std::string a = "ls";
	char b[] = "dir -l /home";
	for (int i = 0; i < 20; i++)
	{
		test_command.push_back(command(a));
		test_command.push_back(command(b));
	}

	test_command.push_back(command("sudo service ssh start"));
	auto it = test_command.container.begin();
	for (int i=0;i<20;i++)
	{
		std::cout << i << ":" << std::endl;
	}
	auto i = test_command.get(19);
	i.execute();
}
