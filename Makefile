CPPC=g++
CPPFLAGS=-Wall -std=c++0x
OBJ= allegroterminal
SRC=main.cpp src/Console.cpp
ALLEGRO_LIBS=allegro-5.0 allegro_font-5.0 allegro_ttf-5.0
INC_DIR=..

all:
	$(CPPC) $(CPPFLAGS) -I$(INC_DIR) $(SRC) -o $(OBJ) `pkg-config --cflags --libs $(ALLEGRO_LIBS)`

clean:
	rm -f $(OBJ)
