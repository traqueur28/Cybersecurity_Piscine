#    Colors
GREY = \e[90m
RED = \e[91m
GREEN = \e[92m
YELLOW = \e[93m
BLUE = \e[34m
PURPLE = \e[95m
CYAN = \e[96m
END = \e[39m

#    Showing command
DISP = TRUE

ifeq ($(DISP),TRUE)
    HIDE =
else
    HIDE = @
endif

#    Includes
INC_PATH	=	inc/
INC_FILES	=
INC			=	$(addprefix $(INC_PATH), $(INC_FILES))

#    Files
SRCS_PATH = src

FILES = scorpion.cpp get_data.cpp

SRCS = $(addprefix $(SRCS_PATH)/, $(FILES))


#    Compilation
NAME = scorpion

TEST = tester

CXX = clang++

CFLAGS = -Wall -Wextra -Werror -ljpeg -std=c++17 -lexif

RM = rm -rf

OBJS_PATH = objs/

OBJS = $(patsubst $(SRCS_PATH)%.s,    $(OBJS_PATH)%.o,    $(SRCS))

#    Rules
all: $(NAME)

$(NAME): $(OBJS_PATH) $(OBJS)
	@ echo "$(BLUE)\n         ***Make $(NAME) ***\n$(END)"
	$(HIDE) $(CXX) $(CFLAGS) $(OBJS) -o $(NAME)
	@ echo "$(GREEN)\n        ---$(NAME) created ---\n$(END)"

$(OBJS_PATH):
	$(HIDE) mkdir -p $(OBJS_PATH)

$(OBJS_PATH)%.o: $(SRCS_PATH)%.s $(INC)
	$(HIDE) $(CC) -s $< -o $@
	@ echo "$(GREEN)[ OK ]$(END) $(CYAN)${<:.s=.o}$(END)"

clean:
	$(HIDE) $(RM) $(OBJS_PATH)
	@ echo "$(PURPLE)\n        *** Clean objects ***\n$(END)"

fclean: clean
	$(HIDE) $(RM) $(NAME) $(TEST) $(NAME_B)
	@ echo "$(RED)\n        *** Remove $(NAME) and $(TEST) ***\n$(END)"

re: fclean all

.PHONY: all clean fclean re