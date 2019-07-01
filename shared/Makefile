# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: niragne <niragne@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/11/06 18:20:16 by ldedier           #+#    #+#              #
#    Updated: 2019/07/01 16:34:16 by niragne          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = shared.a
CC = gcc

SRCDIR   = srcs
OBJDIR   = objs
BINDIR   = .
INCLUDESDIR = includes

OK_COLOR = \x1b[32;01m
EOC = \033[0m

SRCS_NO_PREFIX = test.c

SOURCES = $(addprefix $(SRCDIR)/, $(SRCS_NO_PREFIX))
OBJECTS = $(addprefix $(OBJDIR)/, $(SRCS_NO_PREFIX:%.c=%.o))

INC = -I $(INCLUDESDIR)
INCLUDES_NO_PREFIX = shared.h
INCLUDES = $(addprefix $(INCLUDESDIR)/, $(INCLUDES_NO_PREFIX))

CFLAGS = -Wall -Wextra -Werror $(INC)

ifeq ($(DEBUG), 1)
	CFLAGS += -DDEBUG
	CC += -g3
endif

all: $(BINDIR)/$(NAME)

$(OBJDIR)/%.o : $(SRCDIR)/%.c $(INCLUDES)
	@mkdir -p $(OBJDIR)
	$(CC) -c $< -o $@ $(CFLAGS)

$(BINDIR)/$(NAME): $(OBJECTS)
	@ar rc $@ $^
	@ranlib $(NAME)
	@echo "$(OK_COLOR)$(NAME) linked with success !$(EOC)"

clean:
	@rm -f $(OBJECTS)
	@rm -rf $(OBJDIR)

fclean: clean
	@rm -f $(BINDIR)/$(NAME)

re: fclean all

.PHONY: all clean fclean re
