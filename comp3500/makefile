all:
	make compile
	make timing
	make remove

compile:
	gcc -o run_mp mp.c
	gcc -o run_mt mt.c -pthread
	gcc -o run_serial  serial.c

timing:
	for i in {1..5}; do \
		/usr/bin/time -o mp_timings.txt -a -f 'Total Execution Time: %E\nTotal User Time: %U\n\n' ./run_mp; \
		/usr/bin/time -o mt_timings.txt -a -f 'Total Execution Time: %E\nTotal User Time: %U\n\n' ./run_mt; \
		/usr/bin/time -o serial_timings.txt -a -f 'Total Execution Time: %E\nTotal User Time: %U\n\n' ./run_serial; \
	done;

remove:
	rm ./run*
	rm ./testfile
