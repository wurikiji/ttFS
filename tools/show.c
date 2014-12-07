#include <stdio.h>          
#include <linux/unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <asm/unistd_64.h>
#include "ogh.h"

int main()
{
	int ret;
	int i;
	struct timetable tt[TTFS_MAX_SIZE];
	syscall(__NR_check_timetable, tt, &ret);

	
	for(i = 0; i < ret; i++){
		printf("요일 : %d\n", tt[i].wday);
		printf("시작시간 : %d시 %d분\n", tt[i].st_hour, tt[i].st_min);
		printf("끝시간 : %d시 %d분\n\n", tt[i].end_hour, tt[i].end_min);
	}
	
	return 0;
}
