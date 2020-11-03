#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "info.h"

void perror_and_exit(char *s, int exit_code)
{
	perror(s);
	exit(exit_code);
}

int dec2bin(int num)
{
    int bin = 0, k = 1;

    while (num)
    {
        bin += (num % 2) * k;
        k *= 10;
        num /= 2;
    }

    return bin;
}

int dec2oct(int num)
{
    int bin = 0, k = 1;

    while (num)
    {
        bin += (num % 8) * k;
        k *= 10;
        num /= 8;
    }

    return bin;
}

int dec2hexdec_old(int decimalnum)
{
    long quotient, remainder;
    int i, j = 0;
    char hexadecimalnum[100];

    quotient = decimalnum;

    while (quotient != 0)
    {
        remainder = quotient % 16;
        if (remainder < 10)
        {
            hexadecimalnum[j] = 48 + remainder;
            j = j + 1;
        }
        else
        {
            hexadecimalnum[j] = 55 + remainder;
            j = j + 1;
        }
        quotient = quotient / 16;
    }

    for (i = j; i >= 0; i--)
    {
        printf("%c", hexadecimalnum[i]);
    }
    printf("\n");
    return 0;
}

int dec2hexdec(int decimalNumber) 
{
	long int remainder, quotient;
	int i = 1, j, temp;
	char hexadecimalNumber[100];
	
	quotient = decimalNumber;
	while (quotient != 0) 
    {
		temp = quotient % 16;
		
		if (temp < 10)
		    temp = temp + 48; 
        else
		    temp = temp + 55;
		hexadecimalNumber[i++] = temp;
		quotient = quotient / 16;
	}
	
	for (j = i - 1; j > 0; j--)
	      printf("%c", hexadecimalNumber[j]);
    printf("\n");
	return 0;
}

int dec2one(int num)
{
    for (int i = 0; i < num; i++)
    {   
        printf("0");
    }
    printf("\n");
}

int main()
{
    int sock, listener;
    struct sockaddr_in addr, client_addr;
    char buf[MSG_LEN];
    int bytes_read;

    listener = socket(AF_INET, SOCK_STREAM, 0);
    if(listener < 0)
		perror_and_exit("socket()", 1);
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(SOCK_PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
		perror_and_exit("bind()", 2);

    listen(listener, 1);
	printf("Server is listening...\n");
    
    while(1)
    {
		socklen_t cli_addr_size = sizeof(client_addr);
        sock = accept(listener, (struct sockaddr*) &client_addr, &cli_addr_size);
        if(sock < 0)
			perror_and_exit("accept()", 3);

		printf("Received packet from %s:%d\n\n", 
						inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        while(1)
        {
            bytes_read = recv(sock, buf, MSG_LEN, 0);
            
            if(bytes_read <= 0) break;
            send(sock, buf, bytes_read, 0);
        }
        printf("Recieved number: %s\n", buf);
        int num10 = atoi(buf);
        int num2 = dec2bin(num10);
        int num8 = dec2oct(num10);

        printf("Number [10]: %d\n", num10);
        printf("Number [2]: %d\n", num2);
        printf("Number [8]: %d\n", num8);
        printf("Number [16]: ");
        dec2hexdec(num10);
        printf("Number [1]: ");
        dec2one(num10);
        
    
        close(sock);
    }
    
    return 0;
}
