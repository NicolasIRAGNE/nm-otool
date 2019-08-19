# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: niragne <niragne@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/07/18 12:01:42 by ldedier           #+#    #+#              #
#    Updated: 2019/08/19 12:01:42 by niragne          ###   ########.fr        #
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

DEPS =		ft_nm.h

INCLUDES =	includes

LIBFTDIR =	../libft
LIBFT_INCLUDEDIR = $(LIBFTDIR)/includes
LIBFT = $(LIBFTDIR)/libft.a

LIBMACHODIR = ../libmach-o
LIBMACHO_INCLUDEDIR = $(LIBMACHODIR)/includes
LIBMACHO = $(LIBMACHODIR)/libmach-o.a

SRCS =	main.c options.c options_tools.c options_func.c print.c print_symbols.c

SRC	= $(addprefix $(SRCDIR)/, $(SRCS))
DEP = $(addprefix $(INCLUDES)/,$(DEPS))
INC = -I $(INCLUDES) -I $(LIBFT_INCLUDEDIR) -I $(LIBMACHO_INCLUDEDIR)


all:
	@make all -C $(LIBFTDIR)
	@make all -C $(LIBMACHODIR)
	@make $(NAME)

libcompiled:
	@make $(NAME)

debug:
	@make all DEBUG=1

$(NAME): $(LIBFT) $(LIBMACHO) $(OBJECTS) $(DEP)
	$(CC) -o $(NAME) $(OBJECTS) $(LIBFT) $(LIBMACHO) $(LFLAGS) 
	@echo "$(OK_COLOR)$(NAME) compiled with success$(EOC)"

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(INCLUDES)
	@mkdir -p $(OBJDIR)
	$(CC) -o $@ -c $< $(CFLAGS) $(INC)

$(LIBMACHO):
	make -C $(LIBMACHODIR)

clean:
	@make clean -C $(LIBFTDIR)
	@make clean -C $(LIBMACHODIR)
	@rm -rf $(NAME)

fclean: clean
	@make fclean -C $(LIBFTDIR)
	@make fclean -C $(LIBMACHODIR)
	@rm -rf $(OBJECTS)

re: fclean all
