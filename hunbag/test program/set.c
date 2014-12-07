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


void set_tt(struct timetable *tt)
{
	int i;
	char fin;
	int num;

	printf("입력할 시간표 수 >> ");
	scanf("%d", &num);

	for(i=0; i < num; i++){
		printf("%d번째 시간표\n", i+1);
		
		printf("요일 : ");
		scanf("%d", &tt[i].wday);
		
		printf("Start hour : ");
		scanf("%d", &tt[i].st_hour);
	
		printf("Start minute : ");
		scanf("%d", &tt[i].st_min);

		printf("End hour : ");
		scanf("%d", &tt[i].end_hour);
	
		printf("End minute : ");
		scanf("%d", &tt[i].end_min);
	
		tt[i].st_year = 2014;
		tt[i].st_month = 9;
		tt[i].st_date = 1;
		tt[i].end_year = 2014;
		tt[i].end_mon = 12;
		tt[i].end_date = 31;

	}
}

int main(void)
{
	struct timetable tt[TTFS_MAX_SIZE];
	
	set_tt(tt);
	//syscall(__NR_set_timetable, tt, 2);

	return 0;
}
