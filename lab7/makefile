CC = g++
CFLAGS = -g -c -Wall -std=c++0x 
OBJS = main.o Resolve.o SymbolList.o FileHandler.o

.C.o:
	$(CC) $(CFLAGS) $< -o $@

resolve: $(OBJS)
	$(CC) $(OBJS) -o resolve

Resolve.o: SymbolList.h FileHandler.h Resolve.h

FileHandler.o: SymbolList.h FileHandler.h

SymbolList.o: SymbolList.h

main.o: SymbolList.h Resolve.h FileHandler.h

clean:
	rm *.o resolve

submit:
	make clean
	submit can 3482-lab7 *.C *.h makefile
