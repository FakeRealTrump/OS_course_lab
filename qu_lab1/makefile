all: shell

clean:
	rm *.o
	rm shell

command.o : command.cpp
	g++ --std=c++11 -c command.cpp 

historyList.o : command.o  cycleList.h  historyList.cpp
	g++ --std=c++11 -c historyList.cpp 

historyShell.o: historyList.o historyShell.cpp
	g++ --std=c++11  -c historyShell.cpp 

main.o: main.cpp
	g++ --std=c++11 -c main.cpp

shell: main.o historyShell.o historyList.o command.o
	g++ --std=c++11 main.o   historyList.o command.o historyShell.o  -o shell 
