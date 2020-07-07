#CC = gcc
#CFLAGS = -g -W -Wall -ansi -I..
#LDFLAGS =
#LIBS = -ldl
#OBJS = test.o ../malloc_count.o ../stack_count.o

ALL: ciclad_nogen ciclad_nogend

ciclad_nogen : newciclad/Ciclad.cpp newciclad/CicladAdd.cpp newciclad/CicladRmv.cpp newciclad/transaction.cpp newciclad/concept.cpp newciclad/trienode.cpp	
	g++ -Wextra -Wall -static -std=c++14 -O3 -o $@ $^

ciclad_nogend : newciclad/Ciclad.cpp newciclad/CicladAdd.cpp newciclad/CicladRmv.cpp newciclad/transaction.cpp newciclad/concept.cpp newciclad/trienode.cpp	
	g++ -Wextra -Wall -g -std=c++14 -O3 -o $@ $^

clean :
	rm -f ciclad_nogen ciclad_nogend

.PHONY : clean all
