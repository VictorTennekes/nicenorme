# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: vtenneke <vtenneke@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2020/08/04 18:20:34 by vtenneke      #+#    #+#                  #
#    Updated: 2020/10/29 18:23:23 by tbruinem      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

NAME		=	nicenorme
SRC			=	main.CXX
OBJ			=	$(SRC:%.CXX=%.o)
CXXFLAGS	=	
DEBUG_FLAGS =	
KERNEL		=  $(shell uname)

# COLORS
WHITE   = \x1b[37;01m
CYAN    = \x1b[36;01m
PINK    = \x1b[35;01m
BLUE    = \x1b[34;01m
YELLOW  = \x1b[33;01m
GREEN   = \x1b[32;01m
RED     = \x1b[31;01m
BLACK   = \x1b[30;01m
RESET   = \x1b[0m
ifeq ($(KERNEL), Linux)
	WHITE   = \e[37;01m
	CYAN    = \e[36;01m
	PINK    = \e[35;01m
	BLUE    = \e[34;01m
	YELLOW  = \e[33;01m
	GREEN   = \e[32;01m
	RED     = \e[31;01m
	BLACK   = \e[30;01m
	RESET   = \e[0m
endif

ifeq ($(DEBUG), 1)
	DEBUG_FLAGS += -g -fsanitize=address
endif

all: $(NAME)

%.o: %.c
	$(CXX) $(CXXFLAGS) $(DEBUG_FLAGS) -o $@ -c $<

$(NAME): $(OBJ)
	@echo "$(WHITE)/-----    Compiling $(NAME)        -----\\ $(RESET)"
	$(CXX) $(DEBUG_FLAGS)  -o $(NAME) $^

clean:
	@echo "$(WHITE)/-----    Cleaning $(NAME)        -----\\ $(RESET)"
	rm -f $(OBJ)

fclean: clean
	@echo "$(WHITE)/-----    Fcleaning $(NAME)        -----\\ $(RESET)"
	rm -f $(NAME)

re:	fclean all

.PHONY:	all clean fclean re