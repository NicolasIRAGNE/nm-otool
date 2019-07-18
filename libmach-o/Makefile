# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/07/18 11:31:28 by ldedier           #+#    #+#              #
#    Updated: 2019/07/18 12:19:26 by ldedier          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= libmach-o.a
CC      = gcc

DEBUG ?= 0
UNAME = $(shell uname -m)
BINDIR		= .
SRCDIR		= srcs
OBJDIR		= objs
INCLUDESDIR = includes



LIBFTDIR = ../libft
LIBFT_INCLUDEDIR = includes
LIBFT = $(LIBFTDIR)/libft.a

OK_COLOR = \x1b[32;01m
EOC = \033[0m

SRCS_NO_PREFIX = corrupted.c debug.c fill.c fill32.c fill64.c fill_archive.c \
				 fill_archive_member.c fill_debug.c fill_fat32.c fill_tools.c \
				 free.c init.c options.c options_func.c options_tools.c swap.c \
				 symbols.c symbols_sort.c symbols_sort_inv.c tools.c

INCLUDES_NO_PREFIX = mach_o.h

SOURCES = $(addprefix $(SRCDIR)/, $(SRCS_NO_PREFIX))
OBJECTS = $(addprefix $(OBJDIR)/, $(SRCS_NO_PREFIX:%.c=%.o))
	INCLUDES = $(addprefix $(INCLUDESDIR)/, $(INCLUDES_NO_PREFIX))

INC = -I $(INCLUDESDIR) -I $(LIBFTDIR)/$(LIBFT_INCLUDEDIR)

CFLAGS = -DARCH=\"$(UNAME)\" -Wall -Wextra -Werror $(INC)

ifeq ($(DEBUG), 1)
	CFLAGS += -DDEBUG
	CC += -g3
endif

opti:
	@make all -j8

all:
	@make -C $(LIBFTDIR)
	@make $(BINDIR)/$(NAME)

$(LIBFT):
	make -C $(LIBFTDIR) opti

$(OBJDIR)/%.o : $(SRCDIR)/%.c $(INCLUDES)
	@mkdir -p $(OBJDIR)
	$(CC) -c $< -o $@ $(CFLAGS)

$(BINDIR)/$(NAME): $(OBJECTS)
	@ar rc $@ $^
	@ranlib $(NAME)
	@echo "$(OK_COLOR)$(NAME) linked with success !$(EOC)"
clean:
	@make -C $(LIBFTDIR) clean
	@rm -f $(OBJECTS)
	@rm -rf $(OBJDIR)

fclean: clean
	@make -C $(LIBFTDIR) fclean
	@rm -f $(BINDIR)/$(NAME)

re: fclean all

.PHONY: all clean fclean re
