#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <linux/time.h>
#include "../include/linux/ogh.h"

#if 0
#define TTFS_MAX_SIZE	32

#define TTFS_NOERROR 	0 // no error
#define TTFS_EXCEED 	1 // exceed the size of table 
#define TTFS_NOARGS		2 // blank arguments

#define TTFS_NOCLASS	0 // not a class time
#define TTFS_INCLASS	1 // a class time

struct timetable
{
	int day; // what day (Monday, Tuesday ...)
	int st_hour, end_hour; // start hour & end hour
	int st_min, end_min; //start & end minutes
	int duration; // # of weeks 
}
#endif

SYSCALL_DEFINE0(ogh_test)
{
		printk("OGH TEST IS GOOD\n");
		return 10;
}
struct timetable ttfs[TTFS_MAX_SIZE];
int ttfs_index = 0;

int check_timetable(void)
{
		printk("Current time is \n");
		return 0;
}
EXPORT_SYMBOL(check_timetable);

SYSCALL_DEFINE2(set_timetable, struct timetable*, tt, int, num)
{
	int i;
		/* check conditions */
	if ( ttfs_index + num > TTFS_MAX_SIZE ) return TTFS_EXCEED;
	if (!tt) 	return TTFS_NOARGS;

	/* insert into ttfs table */
	for(i = 0 ;i < num;i++)
	{
		copy_from_user((void*)&ttfs[ttfs_index + i], 
			(const void*)&tt[i], sizeof(struct timetable));
		printk("Date: %d, Start: %02d-%02d, End: %02d-%02d, %d Weeks\n",
						tt[i].day, tt[i].st_hour, tt[i].st_min,
						tt[i].end_hour, tt[i].end_min, tt[i].duration);
	}
	/* update # of tt */
	printk("Update ttfs size %d to %d\n", ttfs_index, ttfs_index + num);
	ttfs_index += num;

	return TTFS_NOERROR;
}


SYSCALL_DEFINE0(check_timetable)
{
		return check_timetable();
}
