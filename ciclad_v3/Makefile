
all: ciclad_v3 ciclad_v3d

ciclad_v3 : ciclad_v3_main.cpp ciclad_v3_impl.cpp
	g++ -Wextra -Wall -static -O3 -o $@ $^

ciclad_v3d : ciclad_v3_main.cpp ciclad_v3_impl.cpp
	g++ -Wextra -Wall -g -O3 -o $@ $^

clean :
	rm -f ciclad_v3 ciclad_v3d

.PHONY : clean all

