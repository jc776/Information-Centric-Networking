# build hello_openwrt executable when user executes "make"
hello_openwrt: hello_openwrt.o
	$(CC) $(LDFLAGS) hello_openwrt.o -o hello_openwrt
hello_openwrt.o: hello_openwrt.c
	$(CC) $(CFLAGS) -c hello_openwrt.c

# remove object files and executable when user executes "make clean"
clean:
	rm *.o hello_openwrt
