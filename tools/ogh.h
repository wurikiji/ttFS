#define TTFS_MAX_SIZE   32

#define TTFS_NOERROR    0 // no error
#define TTFS_EXCEED     1 // exceed the size of table 
#define TTFS_NOARGS     2 // blank arguments

#define TTFS_NOCLASS    0 // not a class time
#define TTFS_INCLASS    1 // a class time   
#define TTFS_NOROOT		2 // need root privilage

struct timetable
{
		int wday; // what day (Monday, Tuesday ...)
		int st_year, st_month, st_date; // start year:month:mday
		int st_hour, end_hour; // start hour & end hour
		int st_min, end_min; //start & end minutes
		int end_year, end_mon, end_date; // end year:month:mday
};
