# -lm added to link math.h
COMPILE_MACRO := gcc -g -lm -ansi -pedantic -Wall

main: datatable.o secondRun.o firstRun.o helpers.o main.c files.o
	$(COMPILE_MACRO) datatable.o helpers.o secondRun.o files.o firstRun.o main.c -o assembler

datatable.o: datatable.c assembler.h
	$(COMPILE_MACRO) -c datatable.c
	
firstRun.o: firstRun.c assembler.h
	$(COMPILE_MACRO) -c firstRun.c

secondRun.o: secondRun.c assembler.h
	$(COMPILE_MACRO) -c secondRun.c

helpers.o: helpers.c assembler.h
	$(COMPILE_MACRO) -c helpers.c

files.o: files.c assembler.h
	$(COMPILE_MACRO) -c files.c

clean:
	-@rm *.o *.ent *.ext *.ob *~ assembler