# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: niragne <niragne@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/06/18 18:36:41 by niragne           #+#    #+#              #
#    Updated: 2019/06/19 12:37:31 by niragne          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_nm

CC = gcc
CFLAGS = -Wall -Wextra -Werror

DEBUG ?= 0

ifeq ($(DEBUG), 1)
	LFLAGS += -L -fsanitize=address
	CFLAGS += -DDEBUG
	CC += -g3
endif

SRCDIR = srcs

OBJDIR   = objs
OBJECTS	= $(addprefix $(OBJDIR)/, $(SRCS:%.c=%.o))

INCLUDES = includes

LIBFTDIR = ../libft
LIBFT_INCLUDEDIR = includes
LIBFT = $(LIBFTDIR)/libft.a

SRCS = main.c

SRC	= $(addprefix $(SRCDIR)/, $(SRCS))

all: $(NAME)

$(NAME): $(LIBFT) $(OBJECTS)
	$(CC) -o $(NAME) $(OBJECTS) $(LIBFT)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(INCLUDES)
	mkdir -p $(OBJDIR)
	$(CC) -o $@ -c $< $(CFLAGS) -I $(INCLUDES) -I $(LIBFT_INCLUDEDIR) $(LFLAGS)

$(LIBFT):
	make -C $(LIBFTDIR)

clean:
	rm -rf $(NAME)

fclean: clean
	rm -rf $(OBJECTS)