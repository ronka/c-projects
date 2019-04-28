mymat: mymat.o mat.o
	gcc -g mymat.o mat.o -o mymat

mat.o: mat.c mat.h
	gcc -c -Wall -ansi -pedantic mat.c

mymat.o: mymat.c mat.h
	gcc -c -Wall -ansi -pedantic mymat.c