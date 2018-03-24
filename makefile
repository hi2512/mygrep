all : mygrep

myfilter.o : myfilter.c
	gcc -c myfilter.c
	
mypattern.o : mypattern.c
	gcc -c mypattern.c

mygrep.o : mygrep.c
	gcc -c mygrep.c

mygrep : myfilter.o mypattern.o mygrep.o
	gcc -o mygrep myfilter.o mypattern.o mygrep.o
	
clean :
	rm mygrep.exe myfilter.o mypattern.o mygrep.o