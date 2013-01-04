M_DIR=Terminal
M_SRCS=Commands.cpp Console.cpp  Window.cpp
SRCS:= $(SRCS) $(addprefix $(M_DIR)/src/,$(M_SRCS))
