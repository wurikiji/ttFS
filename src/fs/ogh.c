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
		int end_year, end_mon, end_date;
}
#endif

SYSCALL_DEFINE0(ogh_test)
{
		printk("OGH TEST IS GOOD\n");
		return 10;
}
struct timetable ttfs[TTFS_MAX_SIZE];
int ttfs_index = 0;

#define TTFS_FILE "/ttfs"

/* clean the timetable */
void truncate_timetable(struct tm cur_tm)
{
	int i;
	int nidx = 0;
	int st_date, end_date, cur_date;
	int fd;
	mm_segment_t old_fs;
	struct file *fp;
	loff_t pos = 0;

	printk("Truncate timetable\n");
	for(i = 0 ;i < ttfs_index;i++)
	{
			if( ttfs[i].st_year <= cur_tm.tm_year && cur_tm.tm_year <= ttfs[i].end_year )
			{
				st_date = ttfs[i].st_month * 30 + ttfs[i].st_date;
				end_date = (ttfs[i].end_year - ttfs[i].st_year) * 365 + 
							ttfs[i].end_mon * 30 + ttfs[i].end_date;
				cur_date = (cur_tm.tm_year - ttfs[i].st_year) * 365 +
							cur_tm.tm_mon * 30 + cur_tm.tm_mday;

				if( st_date <= cur_date && cur_date <= end_date )
				{
						/* valid time table */
						memcpy(&ttfs[nidx++], &ttfs[i], sizeof(struct timetable));
				}
			}
	}
	/* resize time table */
	ttfs_index = nidx;

	/* write time table */
	old_fs = get_fs();
	set_fs(get_ds());

	fd = sys_open(TTFS_FILE, O_RDWR | O_CREAT | O_TRUNC, 0644);
	fp = fget(fd);
	if( fp  && ttfs_index > 0 )
	{
			printk("Write new timetable to file \n");
			vfs_write(fp, (const char *)ttfs, sizeof(struct timetable) * ttfs_index, &pos);
			fput(fp);
	}
	sys_close(fd);
	set_fs(old_fs);
}

/* check the time table */
long check_timetable(struct timetable *tt, int *ret)
{
	int fd;
	mm_segment_t old_fs;
	struct tm today;
	time_t t;
	int st_min, end_min, cur_min;
	int i, rcount;
	long retval = TTFS_NOCLASS;

	old_fs = get_fs();
	set_fs(get_ds());

	fd = sys_open(TTFS_FILE, O_RDWR|O_CREAT, 0644);

	/* succefully opened timetable file */
	if( fd < 0 )
	{
			printk("Failed to open file: %s, err: %d\n", TTFS_FILE, fd);
			return TTFS_NOROOT;
	}
	else
	{
			printk("Succefully opened file %s\n", TTFS_FILE);
	}

	/* read time table from ttfs file */
	if( ttfs_index <= 0 )
	{
			rcount = sys_read(fd, (char*)ttfs, sizeof(struct timetable) * TTFS_MAX_SIZE);
			ttfs_index = (rcount + sizeof(struct timetable) - 1) / sizeof(struct timetable);
	}

	sys_close(fd);
	set_fs(old_fs);

	/* get current time */
	t = get_seconds();
	time_to_tm(t, -1 * (sys_tz.tz_minuteswest * 60) , &today);
	today.tm_mon += 1;
	today.tm_year += 1900;


	/* print current time and date */
	printk("Today is %ld/%02d/%02d-%02d:%02d:%02d (%d)\n", 
			today.tm_year, today.tm_mon, today.tm_mday,
			today.tm_hour, today.tm_min, today.tm_sec,
			today.tm_wday);

	/* truncate finished class */
	truncate_timetable(today);


	/* hour:minute to minutes */
	cur_min = today.tm_hour * 60 + today.tm_min; 

	/* check for all time table */
	printk("Check time table\n");
	for(i = 0; i < ttfs_index; i++)
	{
			/* if same days of a week */
			if( today.tm_wday == ttfs[i].wday )
			{
					st_min = ttfs[i].st_hour * 60 + ttfs[i].st_min;
					end_min = ttfs[i].end_hour * 60 + ttfs[i].end_min;
					/* if in the class time */
					if( st_min <= cur_min && cur_min < end_min)
					{
							retval = TTFS_INCLASS;
							break;
					}
			}
	}

	if( tt && ret)
	{
			printk("Copy Data %d tables\n", ttfs_index);
			copy_to_user(tt, ttfs, sizeof(struct timetable) * ttfs_index);
			copy_to_user(ret, &ttfs_index, sizeof(int));
	}
	return retval;
}
EXPORT_SYMBOL(check_timetable);

SYSCALL_DEFINE2(set_timetable, struct timetable*, tt, int, num)
{
	int i;
	int fd;
	mm_segment_t old_fs;
	struct file *fp;
	loff_t pos = 0;
		/* check conditions */
	if ( ttfs_index + num > TTFS_MAX_SIZE ) return TTFS_EXCEED;
	if (!tt) 	return TTFS_NOARGS;

	old_fs = get_fs();
	set_fs(get_ds());
	fd = sys_open(TTFS_FILE, O_RDWR | O_CREAT | O_TRUNC, 0664);

	if(fd < 0) 
	{
			printk("Failed to open file\n");
			return TTFS_NOROOT;
	}
	printk("Succefully opened file %s\n", TTFS_FILE);

	/* insert into ttfs table */
	for(i = 0 ;i < num;i++)
	{
		copy_from_user((void*)&ttfs[ttfs_index + i], 
			(const void*)&tt[i], sizeof(struct timetable));
		printk("Days of a week: %d, Start: %02d-%02d, End: %02d-%02d\n",
						tt[i].wday, tt[i].st_hour, tt[i].st_min,
						tt[i].end_hour, tt[i].end_min );
		printk("Start date: %d/%02d/%02d, End date: %d/%02d/%02d\n",
						tt[i].st_year, tt[i].st_month, tt[i].st_date,
						tt[i].end_year, tt[i].end_mon, tt[i].end_date);
	}
	/* update # of tt */
	printk("Update ttfs size %d to %d\n", ttfs_index, ttfs_index + num);
	ttfs_index += num;

	if ( num == 0 )
	{
		printk("Reset timetable \n");
		memset(ttfs, 0, sizeof(struct timetable) * TTFS_MAX_SIZE);
		ttfs_index = 0;
	}

	fp = fget(fd);
	if( fp  && ttfs_index > 0 ) 
			vfs_write(fp, (const char *)ttfs, sizeof(struct timetable) * ttfs_index, &pos);
	fput(fp);
	sys_close(fd);
	set_fs(old_fs);
	return TTFS_NOERROR;
}

SYSCALL_DEFINE2(check_timetable, struct timetable* , tt, int*, retval)
{
		return check_timetable(tt, retval);
}
