.Phony:: all
all:
	
	gcc -o serial serial.c
	./serial

	gcc -o mp mp.c
	./mp

	gcc -pthread -o mt mt.c
	./mt

	ls -la
	gcc -o delete deleteTestFiles.c
	./delete
	