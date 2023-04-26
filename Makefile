# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bducrocq <bducrocq@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/25 18:23:10 by amiguez           #+#    #+#              #
#    Updated: 2023/04/26 16:40:47 by bducrocq         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# =====	Binary Name	=====

NAME := ircserv

# -----------------------
# =====		Path	=====
# -----------------------

PATH_SRCS		:=	srcs
PATH_OBJS		:=	.objs
PATH_INCLUDES	:=	includes

# -----------------------
# =====		Files	=====
# -----------------------

LST_SRCS	 :=	main.cpp
SRCS_SRCS	 :=	$(addprefix $(PATH_SRCS)/,$(LST_SRCS))
OBJS_SRCS	 :=	$(addprefix $(PATH_OBJS)/,$(LST_SRCS:.cpp=.o))
        ####
CLASS		 :=	Channel.cpp\
				Client.cpp\
				NumericReplies.cpp\
				Server.cpp
PATH_CLASS	 :=	Class
LST_CLASS	 :=	$(addprefix $(PATH_CLASS)/,$(CLASS))
SRCS_CLASS	 :=	$(addprefix $(PATH_SRCS)/,$(LST_CLASS))
OBJS_CLASS	 :=	$(addprefix $(PATH_OBJS)/,$(LST_CLASS:.cpp=.o))
        ####
CMDS		 :=	auth.cpp\
				invite.cpp\
				join.cpp\
				kick.cpp\
				list.cpp\
				mode.cpp\
				motd.cpp\
				names.cpp\
				oper.cpp\
				part.cpp\
				ping.cpp\
				privmsg.cpp\
				quit.cpp\
				topic.cpp
PATH_CMDS	 :=	Commands
LST_CMDS	 :=	$(addprefix $(PATH_CMDS)/,$(CMDS))
SRCS_CMDS	 :=	$(addprefix $(PATH_SRCS)/,$(LST_CMDS))
OBJS_CMDS	 :=	$(addprefix $(PATH_OBJS)/,$(LST_CMDS:.cpp=.o))
        ###
UTILS		 :=	commands.cpp\
				parser.cpp\
				tools.cpp
PATH_UTILS	 :=	Utils
LST_UTILS	 :=	$(addprefix $(PATH_UTILS)/,$(UTILS))
SRCS_UTILS	 :=	$(addprefix $(PATH_SRCS)/,$(LST_UTILS))
OBJS_UTILS	 :=	$(addprefix $(PATH_OBJS)/,$(LST_UTILS:.cpp=.o))
        ####
LST_INCS	 :=	Class/ANSI.hpp\
				Class/Channel.hpp\
				Class/Client.hpp\
				Class/NumericReplies.hpp\
				Class/Server.hpp\
				tools.hpp

INCS		 := $(addprefix $(PATH_INCLUDES)/,$(LST_INCS))

# -----------------------
# =====	Definition	=====
# -----------------------

SRCS := $(SRCS_SRCS) $(SRCS_UTILS) $(SRCS_CMDS) $(SRCS_CLASS) $(SRCS_CHANNEL)

OBJS := $(OBJS_SRCS) $(OBJS_UTILS) $(OBJS_CMDS) $(OBJS_CLASS) $(OBJS_CHANNEL)

# -----------------------
# =====		Flags	=====
# -----------------------

CPP		 :=	c++ -std=c++98
CPPFLAGS :=	-Wall -Wextra -Werror
SANITIZE :=#	-fsanitize=address
DBGFLAGS :=	-g3

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
	$(CPP) $(SANITIZE) $(DBGFLAGS) $(CPPFLAGS) -I$(PATH_INCLUDES) -Iincludes/Class $(OBJS) -o $@
	echo "$(GREEN)$(NAME) created$(END)"

$(PATH_OBJS)/%.o : $(PATH_SRCS)/%.cpp $(INCS) Makefile | $(PATH_OBJS)
	printf "$(ERASE)$(CYAN)Compilation of $< ...$(END)$(RED)"
	$(CPP) $(SANITIZE) $(DBGFLAGS) -I$(PATH_INCLUDES) -Iincludes/Class $(CPPFLAGS) -o $@ -c $<

$(PATH_OBJS) :
	mkdir -p $(PATH_OBJS)
	mkdir -p $(PATH_OBJS)/$(PATH_CMDS)
	mkdir -p $(PATH_OBJS)/$(PATH_UTILS)
	mkdir -p $(PATH_OBJS)/$(PATH_CLASS)

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