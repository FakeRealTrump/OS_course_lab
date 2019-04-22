#pragma once
#include "FileExamine.h"
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace FileEx {
	class FileExamine_socket_impl :
		public FileExamine
	{
	public:
		FileExamine_socket_impl() = default;
		~FileExamine_socket_impl() = default;
		FileExamine_socket_impl(std::string FilePath) :FileExamine(FilePath) {}
		void examword(std::string Word) override;
	private:
		class Server{
		public:
			Server() {
				memset(&addr, 0, sizeof(addr));
				addr.sin_family = AF_INET; 
				addr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
				addr.sin_port = htons(1728);  

				socket_id = socket(AF_INET, SOCK_STREAM,0);
				bind(socket_id, (struct sockaddr*)&addr, sizeof(addr));
				listen(socket_id, 25);			
			}
			~Server()
			{
				close(socket_id);
			}
			int wait() {
				printf("wait for connect\n");
				socklen_t clnt_addr_size = sizeof(clientaddr);
				conn_id = accept(socket_id, (struct sockaddr*)&clientaddr, &clnt_addr_size);
				printf("connect established\n");
			}
			int checkForWord(string Word)
			{
				char buff[512];
				string aLine;
				int MAXLINE = 512;
				while (1) {
					int n = read(conn_id, buff, MAXLINE);
					buff[n] = '\0';

					aLine = string(buff);
					if (checkWord(aLine, Word))
					{
						cout << aLine << endl;
					}

					if (n == 0)
					{
						printf("All data has been read, the connection will be free now\n");
						break;
					}
				}
			}
			sockaddr_in* getaddr() { return &addr; }
			int getID() { return socket_id; }

		private:
			int socket_id;
			int conn_id;
			sockaddr_in addr;
			sockaddr_in clientaddr;
		};
		class Client {
		public:
			Client() {
				socket_id = socket(AF_INET, SOCK_STREAM, 0);
			}
			~Client()
			{
			//	cout << "oh I'm Client of" << getpid()<<",I'm dead now"<<endl;
				close(socket_id);
			}
			int connectTo(Server S) {
				int res = connect(socket_id, (struct sockaddr*)S.getaddr(), sizeof(sockaddr_in));
				while(res)
				{
					fprintf(stderr, "socket() failed: %s\n", strerror(errno));
					res = connect(socket_id, (struct sockaddr*)S.getaddr(), sizeof(sockaddr_in));
				}
				
			}
			bool writeTo(string Data) {
				write(socket_id, Data.c_str(), Data.size()+1);
			}
			void closeCon() {
				close(socket_id);
			}
		private:
			int socket_id;
			int conn_id;
			sockaddr_in addr;
		};
		static Server server;
		//Client client;
	};
FileExamine_socket_impl::Server	FileExamine_socket_impl::server;

	void FileExamine_socket_impl::examword(std::string Word)
	{
		
		printf("checking for Word: %s\n", Word.c_str());
		fflush(stdout);

		//client = Client();

		pid_t pid = fork();
		if (pid == 0)
		{//child

			server.wait();
			server.checkForWord(Word);
			_exit(0);
		}
		else if (pid > 0)
		{//father
			Client client = Client();
			client.connectTo(server);
			string aLine;
			while (!readfrom.eof())
			{


				//write a line of file to child
				getline(readfrom, aLine);
				client.writeTo(aLine);
			}
			client.closeCon();
		}
		readyForNextExam();
	}

}
