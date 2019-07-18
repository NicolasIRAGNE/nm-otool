# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: niragne <niragne@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/06/18 18:36:50 by niragne           #+#    #+#              #
#    Updated: 2019/07/18 18:42:29 by ldedier          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = proj

PWD = \"$(shell pwd)\"

DEBUG ?= 0

SRCDIR   = srcs
OBJDIR   = objs
BINDIR   = .

NM_NAME = ft_nm
NM_DIR = nm

OTOOL_NAME = ft_otool
OTOOL_DIR = otool

INCLUDESDIR = includes

SPEED = -j8
LIBFTDIR = libft
LIBFT_INCLUDEDIR = includes
LIBFT = $(LIBFTDIR)/libft.a

LIBMACHODIR = libmach-o
LIBMACHO_INCLUDEDIR = $(LIBMACHODIR)/includes
LIBMACHO = $(LIBMACHODIR)/libmach-o.a

INCLUDES_NO_PREFIX	=	nm.h otool.h

SOURCES = $(addprefix $(SRCDIR)/, $(SRCS_NO_PREFIX))
OBJECTS = $(addprefix $(OBJDIR)/, $(SRCS_NO_PREFIX:%.c=%.o))
INCLUDES = $(addprefix $(INCLUDESDIR)/, $(INCLUDES_NO_PREFIX))


INC = -I $(INCLUDESDIR) -I $(LIBFTDIR)/$(LIBFT_INCLUDEDIR) -I $(LIBMACHO_INCLUDEDIR)

CFLAGS = -DPATH=$(PWD) -Wall -Wextra -Werror $(INC)
LFLAGS = -L $(LIBFTDIR)

ifeq ($(DEBUG), 1)
	MFLAGS = debug
	LFLAGS += -fsanitize=address
	CFLAGS += -DDEBUG
	CC += -g3
endif

all:
	@make -C $(LIBFTDIR)
	@make -C $(LIBMACHODIR)
	@make -C $(OTOOL_DIR)
	@make -C $(NM_DIR)
	@cp $(NM_DIR)/$(NM_NAME) .
	@cp $(OTOOL_DIR)/$(OTOOL_NAME) .

$(NAME):
	make $(NAME)

debug:
	make all DEBUG=1

$(NM_NAME): $(NM_DIR)/$(NM_NAME)
	@make -C $(NM_DIR) DEBUG=$(DEBUG)

$(OTOOL_NAME): $(OTOOL_DIR)/$(OTOOL_NAME)
	@make -C $(OTOOL_DIR) DEBUG=$(DEBUG)

$(LIBFT):
	make -C $(LIBFTDIR) DEBUG=$(DEBUG)

$(LIBMACHO):
	make -C $(LIBMACHODIR)

clean:
	make -C $(NM_DIR) clean
	make -C $(OTOOL_DIR) clean

fclean:
	make -C $(NM_DIR) fclean
	make -C $(OTOOL_DIR) fclean
	rm -rf $(NM_NAME)
	rm -rf $(OTOOL_NAME)

re: fclean all

.PHONY: all clean fclean re debug
