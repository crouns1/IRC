NAME = irc_server
SRC = Server.cpp CommandHandler.cpp parser.cpp main.cpp Client.cpp
OBJS = $(SRC:.cpp=.o)
CC = c++ 
CFLAGS = -Wall -Wextra -Werror -g -std=c++98

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o:%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean: clean 
	rm -rf $(NAME)

re: fclean all 

run: all
	./$(NAME) 6667 testpassword

.PHONY: all re fclean clean run


test_client: test_client.cpp
	$(CC) $(CFLAGS) test_client.cpp -o test_client

test: all test_client
	@echo "Starting server in background..."
	@./$(NAME) 6667 testpassword &
	@sleep 2
	@echo "Running test client..."
	@./test_client || true
	@echo "Test complete"