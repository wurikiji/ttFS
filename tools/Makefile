set=set.o
show=show.o
write=write_test.o
CFLAGS=-I/home/ogh/work/git/ttFS/ulinux/linux-3.10.61/usr/include/


ALL: $(set) $(show) $(write)
	gcc -o set $(set) $(CFLAGS)
	gcc -o show $(show) $(CFLAGS)
	gcc -o write $(write) $(CFLAGS)
