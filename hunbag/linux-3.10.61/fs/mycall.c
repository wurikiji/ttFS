#include <linux/unistd.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/sched.h>

asmlinkage int sys_mycall(int n)
{
	printk("do it mycall(int n)\n");
	return n*n;
}

