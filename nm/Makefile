# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: niragne <niragne@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/06/18 18:36:41 by niragne           #+#    #+#              #
#    Updated: 2019/06/20 15:58:21 by niragne          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_nm

CC = gcc
CFLAGS = -Wall -Wextra -Werror

DEBUG ?= 0

ifeq ($(DEBUG), 1)
	LFLAGS += -fsanitize=address
	CFLAGS += -DDEBUG -fsanitize=address
	CC += -g3
endif

SRCDIR = srcs
SPEED = -j8
OBJDIR   = objs
OBJECTS	= $(addprefix $(OBJDIR)/, $(SRCS:%.c=%.o))

INCLUDES = includes

LIBFTDIR = ../libft
LIBFT_INCLUDEDIR = $(LIBFTDIR)/includes
LIBFT = $(LIBFTDIR)/libft.a

SRCS = debug.c main.c

SRC	= $(addprefix $(SRCDIR)/, $(SRCS))

all: $(NAME)

debug: 
	make all DEBUG=1

$(NAME): $(LIBFT) $(OBJECTS)
	$(CC) -o $(NAME) $(OBJECTS) $(LIBFT) $(LFLAGS)
	@echo "$(NAME) compiled with success"

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(INCLUDES)
	@mkdir -p $(OBJDIR)
	$(CC) -o $@ -c $< $(CFLAGS) -I $(INCLUDES) -I $(LIBFT_INCLUDEDIR)

$(LIBFT):
	make -C $(LIBFTDIR)

clean:
	rm -rf $(NAME)

fclean: clean
	rm -rf $(OBJECTS)

re: fclean all
