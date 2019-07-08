# -lm added to link math.h
COMPILE_MACRO := gcc -g -lm -ansi -pedantic -Wall

main: firstRun.o helpers.o main.c
	$(COMPILE_MACRO) helpers.o firstRun.o main.c -o assembler

firstRun.o: firstRun.c assembler.h
	$(COMPILE_MACRO) -c firstRun.c

helpers.o: helpers.c assembler.h
	$(COMPILE_MACRO) -c helpers.c

clean:
	-@rm *.o *~ assembler

# main: main.c symbollocation.o instructionqueue.o list.o memory.o logging.o codesection.o datasection.o parser.o symboltable.o opcodemap.o
# 	$(COMPILE_MACRO) symbollocation.o instructionqueue.o list.o memory.o logging.o codesection.o datasection.o opcodemap.o symboltable.o parser.o main.c -o main

# parser.o: parser.c parser.h
# 	$(COMPILE_MACRO) -c parser.c
	
# symboltable.o: symboltable.c symboltable.h
# 	$(COMPILE_MACRO) -c symboltable.c

# opcodemap.o: opcodemap.c opcodemap.h	
# 	$(COMPILE_MACRO) -c opcodemap.c

# datasection.o: datasection.c datasection.h
# 	$(COMPILE_MACRO) -c datasection.c

# codesection.o: codesection.c codesection.h
# 	$(COMPILE_MACRO) -c codesection.c

# logging.o: logging.c logging.h
# 	$(COMPILE_MACRO) -c logging.c

# memory.o: memory.c memory.h
# 	$(COMPILE_MACRO) -c memory.c

# list.o: list.c list.h
# 	$(COMPILE_MACRO) -c list.c

# instructionqueue.o: instructionqueue.c instructionqueue.h
# 	$(COMPILE_MACRO) -c instructionqueue.c

# symbollocation.o: symbollocation.c symbollocation.h
# 	$(COMPILE_MACRO) -c symbollocation.c

# clean:
# 	# the -@ is so the make wont fail it threre were no files to delete
# 	-@rm *.o *~ main
