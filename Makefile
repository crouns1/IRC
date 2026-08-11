NAME		= ircserv

CXX			= c++
CXXFLAGS	= -Wall -Wextra -Werror -std=c++98
INCLUDES	= -I include

SRC_DIR		= src
OBJ_DIR		= obj
HEADERS_DIR = include

SRC			= $(SRC_DIR)/main.cpp \
			  $(SRC_DIR)/Server.cpp \
			  $(SRC_DIR)/ServerNet.cpp \
			  $(SRC_DIR)/ServerC.cpp \
			  $(SRC_DIR)/ServerCh.cpp \
			  $(SRC_DIR)/ServerMan.cpp \
			  $(SRC_DIR)/CommandHandler.cpp \
			  $(SRC_DIR)/parser.cpp \
			  $(SRC_DIR)/Client.cpp \
			  $(SRC_DIR)/Channel.cpp

OBJ			= $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

HEADERS		= $(HEADERS_DIR)/Channel.hpp \
					$(HEADERS_DIR)/Client.hpp \
					$(HEADERS_DIR)/CommandHandler.hpp \
					$(HEADERS_DIR)/Server.hpp \
					$(HEADERS_DIR)/Tools.hpp

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS) | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

run: all
	./$(NAME) 6667 testpassword

.PHONY: all clean fclean re run
