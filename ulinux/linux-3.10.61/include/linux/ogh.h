#define TTFS_MAX_SIZE   32

#define TTFS_NOERROR    0 // no error
#define TTFS_EXCEED     1 // exceed the size of table 
#define TTFS_NOARGS     2 // blank arguments

#define TTFS_NOCLASS    0 // not a class time
#define TTFS_INCLASS    1 // a class time   

struct timetable
{
		int day; // what day (Monday, Tuesday ...)
		int st_hour, end_hour; // start hour & end hour
		int st_min, end_min; //start & end minutes
		int duration; // # of weeks 
};
