// TCP는 데이터 경계가 없다 -> 내가 보고싶은 크기만큼 볼 수 있다

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
	int str_len = 0;

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

	// 한 클자씩 읽겠다.
	int idx = 0, read_len = 0;
	while(read_len=read(sock, &message[idx++],1))
	{
		if(read_len == -1)
		{
			error_handling("read() error");
		}

		str_len+=read_len;
	}


	if(str_len == -1)
	{
		error_handling("read() error");
	}

	printf("Message from server : %s \n", message);
	printf("Function read call count : %d \n", str_len);

	close(sock);

	return 0;

}


void error_handling(char* message)
{
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}

