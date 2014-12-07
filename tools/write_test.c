#include <stdio.h>
#include <string.h>
#include <linux/unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <asm/unistd_64.h>
#include "ogh.h"

#define BUFF_SIZE 1024

int main(void)
{
	int fd;
	int cont;
	char* temp = "ABCDEFG\n";
	char buff[BUFF_SIZE];

	if( 0 < (fd = open("/opt/test.txt", O_RDWR | O_CREAT, 0644)))
	{
		while(1){
			printf("write test\n");
			if(write(fd, temp, strlen(temp)) == -1){
				printf("write failed\n");
			}
			
			printf("\nread test\n");
			if(read(fd, buff, BUFF_SIZE) == -1){
				printf("read failed\n");
			}
		
			printf("continue?  \n");
			cont = getchar();
			if(cont == EOF)
				break;
		}
		close(fd);
	}else
		printf("file open failed\n");

	return 0;
}
