# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: amiguez <amiguez@student.42lyon.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/22 02:28:45 by bducrocq          #+#    #+#              #
#    Updated: 2023/04/11 14:45:49 by amiguez          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# =====	Binary Name	=====

NAME = Bob

# -----------------------
# =====		Path	=====
# -----------------------

PATH_SRCS		:=	srcs
PATH_OBJS		:=	.objs
PATH_INCLUDES	:=	includes

# -----------------------
# =====		Files	=====
# -----------------------

LST_SRCS	 :=	main.cpp\
				bot.cpp\
				commands.cpp
SRCS_SRCS	 :=	$(addprefix $(PATH_SRCS)/,$(LST_SRCS))
OBJS_SRCS	 :=	$(addprefix $(PATH_OBJS)/,$(LST_SRCS:.cpp=.o))
        ####
LST_INCS	 :=	Bot.hpp
INCS		 :=	$(addprefix $(PATH_INCLUDES)/,$(LST_INCS))

# -----------------------
# =====	Definition	=====
# -----------------------

SRCS	 :=	$(SRCS_SRCS) 
OBJS	 :=	$(OBJS_SRCS) 

# -----------------------
# =====		Flags	=====
# -----------------------

CPP		 :=	c++ -std=c++98
CPPFLAGS :=	-Wall -Wextra #-Werror
SANITIZE :=	-fsanitize=address -g3

# -----------------------
# =====	Cosmetics	=====
# -----------------------

ERASE	 :=	\033[2K\r
GREY	 :=	\033[30m
RED		 :=	\033[31m
GREEN	 :=	\033[32m
YELLOW	 :=	\033[33m
BLUE	 :=	\033[34m
PINK	 :=	\033[35m
CYAN	 :=	\033[36m
WHITE	 :=	\033[37m
BOLD	 :=	\033[1m
UNDER	 :=	\033[4m
SUR		 :=	\033[7m
END		 :=	\033[0m

# -----------------------
# =====		Rules	=====
# -----------------------

all : $(NAME)

$(NAME) : $(OBJS)
	echo "$(BLUE)Creation of $(NAME) on linux ...$(END)"
	$(CPP) $(CPPFLAGS) $(OBJS) -I$(PATH_INCLUDES) -o $@
	echo "$(GREEN)$(NAME) created$(END)"

$(PATH_OBJS)/%.o : $(PATH_SRCS)/%.cpp $(INCS) Makefile | $(PATH_OBJS)
	printf "$(ERASE)$(CYAN)Compilation of $< ...$(END)$(RED)"
	$(CPP) $(CPPFLAGS) -I$(PATH_INCLUDES) -o $@ -c $<

$(PATH_OBJS) :
	mkdir -p $(PATH_OBJS)
	mkdir -p $(PATH_OBJS)/$(PATH_SERVER)
	mkdir -p $(PATH_OBJS)/$(PATH_CLIENT)
	mkdir -p $(PATH_OBJS)/$(PATH_UTILS)
 #	 mkdir -p $(PATH_OBJS)/$(PATH_CHANNEL)

clean :
	echo "$(BLUE)Removal of .o files of $(NAME) ...$(END)"
	rm -rf $(PATH_OBJS)
	echo "$(RED)Files .o deleted\n$(END)"

fclean : clean
	echo "$(BLUE)Removal of $(NAME)...$(END)"
	rm -rf $(NAME)
	echo "$(RED)Binary $(NAME) deleted\n$(END)"

git :
	git add .
	printf "Message of the commit: " && read msg && git commit -m "$$msg"
	git push

re : fclean all

.PHONY : all, clean, fclean, re, $(PATH_OBJS), git
.SILENT :