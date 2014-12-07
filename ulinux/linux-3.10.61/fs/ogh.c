#include <linux/syscalls.h>
#include <linux/init.h>
#include <linux/fcntl.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/kernel.h>
#include <linux/time.h>
#include <asm/segment.h>
#include <linux/time.h>
#include "../include/linux/ogh.h"
#include <linux/buffer_head.h>

#if 0
#define TTFS_MAX_SIZE	32

#define TTFS_NOERROR 	0 // no error
#define TTFS_EXCEED 	1 // exceed the size of table 
#define TTFS_NOARGS		2 // blank arguments

#define TTFS_NOCLASS	0 // not a class time
#define TTFS_INCLASS	1 // a class time

struct timetable
{
	int wday; // what day (Monday, Tuesday ...)
	int st_year, st_month, st_date; // start year:month:date
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

#define TTFS_FILE "/etc/ttfs"
int check_timetable(void)
{
	int fd, err;
	struct file* fp;
	mm_segment_t old_fs;
	struct tm today;
	time_t t;

	old_fs = get_fs();
	set_fs(get_ds());

	fp = NULL;
	fp = filp_open(TTFS_FILE, O_RDWR|O_CREAT, 0644);
	fd = sys_open(TTFS_FILE, O_RDWR|O_CREAT, 0664);
	
	if( IS_ERR(fp) )
	{
			err = PTR_ERR(fp);
			printk("Error open using fp %d\n", err);
	}
	else {
			printk("Success open using fp\n");
			filp_close(fp, NULL);
	}
	if( fd >= 0) 
	{
			printk("Succefully opened file %s\n", TTFS_FILE);
			sys_close(fd);
	}
	else{
			printk("Failed to open file\n");
	}
	
	t = get_seconds();
	time_to_tm(t, -1 * (sys_tz.tz_minuteswest * 60) , &today);
	printk("Today is %ld/%02d/%02d-%02d:%02d:%02d (%d)\n", 
			today.tm_year, today.tm_mon, today.tm_mday,
			today.tm_hour, today.tm_min, today.tm_sec,
			today.tm_wday);
	set_fs(old_fs);
	return 0;
}
EXPORT_SYMBOL(check_timetable);

SYSCALL_DEFINE2(set_timetable, struct timetable*, tt, int, num)
{
	int i;
	int fd;
	mm_segment_t old_fs;
		/* check conditions */
	if ( ttfs_index + num > TTFS_MAX_SIZE ) return TTFS_EXCEED;
	if (!tt) 	return TTFS_NOARGS;

	old_fs = get_fs();
	set_fs(get_ds());
	fd = sys_open(TTFS_FILE, O_RDWR|O_CREAT, 0664);
	
	if( fd >= 0) 
	{
			printk("Succefully opened file %s\n", TTFS_FILE);
			sys_close(fd);
	}
	else{
			printk("Failed to open file\n");
	}
	set_fs(old_fs);
	/* insert into ttfs table */
	for(i = 0 ;i < num;i++)
	{
		copy_from_user((void*)&ttfs[ttfs_index + i], 
			(const void*)&tt[i], sizeof(struct timetable));
		printk("Date: %d, Start: %02d-%02d, End: %02d-%02d, %d Weeks\n",
						tt[i].wday, tt[i].st_hour, tt[i].st_min,
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
