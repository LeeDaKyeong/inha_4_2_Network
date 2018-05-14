// 멀티 프로세스를 멀티 플렉싱으로

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char *message);
void read_routine(int sock, char *buf);
void write_routine(int sock, char *buf);

int main(int argc, char *argv[])
{
	int sock;
	//pid_t pid;
	char buf[BUF_SIZE];
	struct sockaddr_in serv_adr;
	if(argc != 3)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_adr.sin_port=htons(atoi(argv[2]));

	if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
		error_handling("connect() error!");

	// pid = fork();
	// if(pid == 0)
	// 	write_routine(sock, buf);
	// else
	// 	read_routine(sock, buf);

	// 키보드 입력 표준 0으로 set
	// while문 안에 타임아웃
	// isset으로 리드, 라이트 둘다 확인

	// 1. 정의 : 초기화
	// 2. 등록

	//while
	
		// 3. timeout 설정
		// 4. 

		// if(FD_ISSET()) 
		// 	//read
		// if(FD_ISSET())
		// 	//write

	close(sock);
	return 0;

}

void read_routine(int sock, char *buf)
{
	//while(1) // while -> 반복문으로 select. 변화가 생겼을 때만 수행하도록!
	//{
		int str_len = read(sock, buf, BUF_SIZE);
		if(str_len == 0)
			return;
		else sleep(3); // fork안쓰면 hello보내고 데이터가 돌아올 때까지 3초를 기다리고, 이후에 다시 데이터 보낼 수 있음 -> 
						// 여기서는 보내는거 따로 받는거 따로니까 대답 안와도 계속 보낼 수 있음

		buf[str_len] = 0;
		printf("Message form server : %s", buf);
	//}
}

void write_routine(int sock, char *buf)
{
	//while(1)
	//{
		fgets(buf, BUF_SIZE, stdin);
		if(!strcmp(buf, "q\n") || !strcmp(buf, "Q\n"))
		{
			shutdown(sock, SHUT_WR);
			return;
		}
		write(sock, buf, strlen(buf));
	//}
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
