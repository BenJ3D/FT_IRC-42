# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bducrocq <bducrocq@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/22 02:28:45 by bducrocq          #+#    #+#              #
#    Updated: 2023/04/17 00:09:19 by bducrocq         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Binary

NAME = exec_ircserv

# Path

SRC_PATH = ./srcs/
SRC_EXT = cpp
INC_EXT = hpp

OBJ_PATH = ./.objs/

CPPFLAGS = -I./includes/

HEADER =	$(shell find $(SRC_PATH) -name '*.$(INC_EXT)')
# HEADER =	$(wildcard ./includes/*.hpp)

# Variable d'inclusion
# INC = -I./includes/


# Name

SRC_NAME =	$(shell find $(SRC_PATH) -name '*.$(SRC_EXT)')

OBJ_NAME = $(patsubst $(SRC_PATH)%,$(OBJ_PATH)%,$(SRC_NAME:.cpp=.o))

# Files

SRC = $(SRC_NAME)

OBJ = $(OBJ_NAME)

# Flags

CC = c++ $(STDCPP) $(CFLAGS) $(SANITIZE) $(LLDBFLAG)

CFLAGS = -Wall -Wextra# -Werror
SANITIZE = #-fsanitize=address
LLDBFLAG = -g3
STDCPP = -std=c++98

# Rules

all: $(NAME)

$(NAME): $(OBJ)
	@echo "\033[34mCreation of $(NAME) on linux ...\033[0m"
	@$(CC) $(OBJ) -o $@
	@echo "\033[32m$(NAME) created\n\033[0m"

$(OBJ_PATH)%.o: $(SRC_PATH)%.$(SRC_EXT) $(HEADER) ./Makefile
	@mkdir -p $(@D) 2> /dev/null || true
	$(CC) $(CPPFLAGS) -o $@ -c $<

clean:
	@echo "\033[33mRemoval of .o files of $(NAME) ...\033[0m"
	@rm -f $(OBJ)
	@find $(OBJ_PATH) -type d -empty -delete 2> /dev/null || true
	@echo "\033[31mFiles .o deleted\n\033[0m"

fclean: clean
	@echo "\033[33mRemoval of $(NAME)...\033[0m"
	@rm -rf $(NAME)
	@echo "\033[31mBinary $(NAME) deleted\n\033[0m"

git:
	@git add .
	@printf "Message of the commit: " && read msg && git commit -m "$$msg"
	@git push

re: fclean all

.PHONY: all, clean, fclean, re
