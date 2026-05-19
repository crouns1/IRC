NAME = parsing_IRC
SRC = CommandHandler.cpp parsing.cpp main.cpp 
OBJS = $(SRC:.cpp=.o)
CC = c++ 
CFLAGS = -Wall -Wextra -Werror -g -std=c++98

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o:%.cpp CommandHandler.hpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean: clean 
	rm -rf $(NAME)

re: fclean all 

run: re clean 
	./$(NAME) 

.PHONY: all re fclean clean 
