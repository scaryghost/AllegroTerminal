include config.mk

all: $(OBJS)
	$(CPPC) $(CPP_FLAGS) $(MAIN) -o $(MAIN_OBJ) $(OBJS) `pkg-config --cflags --libs $(ALLEGRO_LIBS)`

%.o: %.cpp
	$(CPPC) -c $(CPP_FLAGS) $< -o $@

clean:
	rm -f $(OBJS) $(MAIN_OBJ)
