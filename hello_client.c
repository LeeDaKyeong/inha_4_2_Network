// client
// gcc hello_client.c -o hclient
// ./hclient 127.0.0.1 9190

#include <stdio.h> //scanf, printf .. IO function
#include <stdlib.h> //atoi function(string->int)
#include <string.h> //memset func
#include <unistd.h> //read, write, close func
#include <arpa/inet.h> //htons, htonl, ..
#include <sys/socket.h>

void error_handling(char * message);

int main(int argc, char * argv[])
{
	int sock;
	struct sockaddr_in serv_addr;
	char message[30];
	int str_len;

	if(argc != 3)
	{
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET,SOCK_STREAM,0);
	if(sock == -1) //실패하면 -1반환
	{
		error_handling("SOCKET ERROR");

	}

	memset(&serv_addr,0,sizeof(serv_addr)); //0으로 serv_addr을 초기화
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]); //요고 중요!!
	serv_addr.sin_port = htons(atoi(argv[2]));

	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
	{
		error_handling("connet error");
	}

	str_len = read(sock,message,sizeof(message));

	if(str_len == -1)
	{
		error_handling("read() error");
	}

	printf("Message from server : %s \n", message);

	close(sock);

	return 0;

}


void error_handling(char* message)
{
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}


