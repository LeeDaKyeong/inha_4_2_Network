#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 100

void error_handling(char *message);
void read_routine(int sock, char *buf);
void write_routine(int sock, char *buf);

int main(int argc, char *argv[])
{
	int sock;
	char buf[BUF_SIZE];
	struct sockaddr_in serv_adr;
	
	struct timeval timeout; // timeout 정의
	fd_set reads, cpy_reads; // fd_set 정의

	int fd_max, str_len, fd_num;

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


	FD_ZERO(&reads); // fd_set zero 초기화
	FD_SET(0, &reads); // 처음에 바로 입력하기 때문에 표준 입력 1로 set
	FD_SET(sock, &reads); // sock 연결 -> set

	fd_max = sock; // 검사범위 지정

	while(1)
	{
		cpy_reads = reads; // fd_set copy
		timeout.tv_sec = 5; // timeout 설정
		timeout.tv_usec = 5000;

		if((fd_num = select(fd_max+1, &cpy_reads, 0, 0, &timeout))==-1) // select 함수 호출로 fd_max+1에 변화 감지
			break;
		else if(fd_num == 0) // time-out 발생
		{
			puts("Time-out!");
			continue;
		}
		else // 에러 x, timeout x 일때
		{
			if(FD_ISSET(sock,&cpy_reads)) // sock이 1이라면 read
			{
				read_routine(sock, buf);
			}
			if(FD_ISSET(0,&cpy_reads)) // 입력이 발생했다면 write
			{	
				write_routine(sock, buf);
			}
		}

	}

	close(sock);
	return 0;

}

void read_routine(int sock, char *buf)
{
	int str_len = read(sock, buf, BUF_SIZE);
	if(str_len == 0)
		return;

	buf[str_len] = 0;
	printf("Message form server : %s", buf);
}

void write_routine(int sock, char *buf)
{
	fgets(buf, BUF_SIZE, stdin);
	if(!strcmp(buf, "q\n") || !strcmp(buf, "Q\n"))
	{
		shutdown(sock, SHUT_WR);
		return;
	}
	write(sock, buf, strlen(buf));
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
