#include <linux/syscalls.h>
#include <linux/kernel.h>

SYSCALL_DEFINE0(ogh_test)
{
		printk("OGH TEST IS GOOD\n");
		return 10;
}

