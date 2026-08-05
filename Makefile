NAME = irc_server
SRC = Server.cpp CommandHandler.cpp parser.cpp main.cpp Client.cpp channel.cpp
OBJS = $(SRC:.cpp=.o)
CC = c++ 
CFLAGS = -Wall -Wextra -Werror -g -std=c++98

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o:%.cpp *.hpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean: clean 
	rm -rf $(NAME)

re: fclean all 

run: all clean
	./$(NAME) 6667 testpassword

.PHONY: all re fclean clean run
