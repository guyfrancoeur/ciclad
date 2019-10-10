# Sample ... :)
#CC = gcc
#CFLAGS = -g -W -Wall -ansi -I..
#LDFLAGS =
#LIBS = -ldl
#OBJS = test.o ../malloc_count.o ../stack_count.o

all: ciclad_v3 ciclad_v3d

#%.o: %.c
#	$(CC) $(CFLAGS) -c -o $@ $<
#
#test: $(OBJS)
#	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)
#

ciclad_v3 : ciclad_v3_main.cpp ciclad_v3_impl.cpp
	g++ -Wextra -Wall -static -std=c++14 -O3 -o $@ $^

ciclad_v3d : ciclad_v3_main.cpp ciclad_v3_impl.cpp
	g++ -Wextra -Wall -g -std=c++14 -O3 -o $@ $^

clean :
	rm -f ciclad_v3 ciclad_v3d

.PHONY : clean all
