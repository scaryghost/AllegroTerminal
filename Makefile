CPPC=g++
CPPFLAGS=-Wall -std=c++0x
OBJ= allegroterminal
SRC=main.cpp
ALLEGRO_LIBS=allegro-5.0 allegro_font-5.0 allegro_ttf-5.0

all:
	$(CPPC) $(CPPFLAGS) $(SRC) -o $(OBJ) `pkg-config --cflags --libs $(ALLEGRO_LIBS)`

clean:
	rm -f $(OBJ)
