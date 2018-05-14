// server
// server 부터 실행시킬것!
// gcc hello_server.c -o hserver
// ./hserver 9190

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char *message);

int main(int argc, char * argv[])
{
	int serv_sock;
	int clnt_sock; 
	// 서버소켓에 클라이언트 소켓이왜? -> 서버는 클라이언트로부터 요청, 서로 서비스를 하려면 서비르스를 위한 소켓이 필요
	// 클라이언트로부터 요청이 들어왔을때 그 클라이언트와 매핑시켜줄 clnt_sock필요
	// serv_sock은 문지기역할

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;

	char message[] = "hello world!";

	if(argc != 2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	// scoket만드는 과정
	serv_sock = socket(PF_INET, SOCK_STREAM,0); //성공하면 0반환
	if(serv_sock == -1) //실패하면 -1반환
	{
		error_handling("Socket() error");

	}

	// IP주소와 Port설정
	// 전화번호 부여하는법
	memset(&serv_addr,0,sizeof(serv_addr)); //0으로 serv_addr을 초기화
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); //hton : host to network // 데이터는 바꿔주지않아도 내부적으로 알아서. 주소만 바꿔줄것
	serv_addr.sin_port = htons(atoi(argv[1])); // atoi : string to int coversion function

	if(bind(serv_sock,(struct sockaddr*) &serv_addr, sizeof(serv_addr))==-1)
	{
		error_handling("bind() error");
	}


	//
	if(listen(serv_sock,5)==-1)
	{
		error_handling("listen() error");
	}

	clnt_addr_size = sizeof(clnt_addr);
	clnt_sock = accept(serv_sock,(struct sockaddr*)&clnt_addr, &clnt_addr_size);
	// clnt_addr 정의한 적 x -> accept함수가 내부적으로 clnt_addr에 할당

	if(clnt_sock == -1)
	{
		error_handling("accept() error");
	}

	write(clnt_sock,message,sizeof(message));

	close(clnt_sock);
	close(serv_sock);


	return 0;

}


void error_handling(char* message)
{
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}


