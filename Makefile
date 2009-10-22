CC=g++
main: main.o remote_logging.o


.PHONY: clean

clean:
	-rm -fv *.o main
