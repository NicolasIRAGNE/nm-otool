# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: niragne <niragne@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/06/18 18:36:41 by niragne           #+#    #+#              #
#    Updated: 2019/07/01 16:38:44 by niragne          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_otool

CC = gcc

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

DEPS =		ft_otool.h

INCLUDES = includes

LIBFTDIR = ../libft
LIBFT_INCLUDEDIR = $(LIBFTDIR)/includes
LIBFT = $(LIBFTDIR)/libft.a

SHAREDLIB = ../shared
SHAREDLIB_INCLUDEDIR = $(SHAREDLIB)/includes
SHARED = $(SHAREDLIB)/shared.a

SRCS =	main.c print.c

SRC	= $(addprefix $(SRCDIR)/, $(SRCS))
DEP = $(addprefix $(INCLUDES)/,$(DEPS))

all:
	@make -C $(LIBFTDIR) all
	@make $(NAME)

debug:
	make all DEBUG=1

$(NAME): $(LIBFT) $(SHARED) $(OBJECTS) $(DEP)
	$(CC) -o $(NAME) $(OBJECTS) $(LIBFT) $(SHARED) $(LFLAGS)
	@echo "$(OK_COLOR)$(NAME) compiled with success$(EOC)"

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(INCLUDES) $(DEP)
	@mkdir -p $(OBJDIR)
	$(CC) -o $@ -c $< $(CFLAGS) -I $(INCLUDES) -I $(LIBFT_INCLUDEDIR) -I $(SHAREDLIB_INCLUDEDIR)

$(LIBFT):
	make -C $(LIBFTDIR)

clean:
	make clean -C $(LIBFTDIR)
	rm -rf $(NAME)

fclean: clean
	make fclean -C $(LIBFTDIR)
	rm -rf $(OBJECTS)

re: fclean all
