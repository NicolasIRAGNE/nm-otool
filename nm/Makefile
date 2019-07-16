# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: niragne <niragne@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/06/18 18:36:41 by niragne           #+#    #+#              #
#    Updated: 2019/07/16 09:33:34 by ldedier          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_nm

CC = gcc

UNAME = $(shell uname -m)

DEBUG ?= 0
OK_COLOR = \x1b[32;01m
EOC = \033[0m


CFLAGS = -DARCH=\"$(UNAME)\" -Wall -Wextra -Werror

ifeq ($(DEBUG), 1)
	LFLAGS += -fsanitize=address
	CFLAGS += -DDEBUG -fsanitize=address
	CC += -g3
endif

SRCDIR = srcs
SPEED = -j8
OBJDIR   = objs
OBJECTS	= $(addprefix $(OBJDIR)/, $(SRCS:%.c=%.o))

DEPS =		ft_nm.h symbols.h sections.h

INCLUDES = includes

LIBFTDIR = ../libft
LIBFT_INCLUDEDIR = $(LIBFTDIR)/includes
LIBFT = $(LIBFTDIR)/libft.a

SRCS =	debug.c main.c symbols.c symbols_sort.c options.c options_tools.c \
		options_func.c fill.c fill_tools.c fill32.c fill64.c fill_debug.c \
		fill_fat32.c print.c init.c swap.c corrupted.c free.c fill_archive.c \
		symbols_sort_inv.c

SRC	= $(addprefix $(SRCDIR)/, $(SRCS))
DEP = $(addprefix $(INCLUDES)/,$(DEPS))

all:
	@make -C $(LIBFTDIR) all
	@make $(NAME)

debug:
	make all DEBUG=1

$(NAME): $(LIBFT) $(OBJECTS) $(DEP)
	$(CC) -o $(NAME) $(OBJECTS) $(LIBFT) $(LFLAGS)
	@echo "$(OK_COLOR)$(NAME) compiled with success$(EOC)"

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(INCLUDES)
	@mkdir -p $(OBJDIR)
	$(CC) -o $@ -c $< $(CFLAGS) -I $(INCLUDES) -I $(LIBFT_INCLUDEDIR)

$(LIBFT):
	make -C $(LIBFTDIR)

clean:
	make clean -C $(LIBFTDIR)
	rm -rf $(NAME)

fclean: clean
	make fclean -C $(LIBFTDIR)
	rm -rf $(OBJECTS)

re: fclean all
