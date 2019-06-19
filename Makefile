# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: niragne <niragne@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/06/18 18:36:50 by niragne           #+#    #+#              #
#    Updated: 2019/06/19 12:36:11 by niragne          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME = 

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
LIBFTDIR = libft
SPEED = -j8
LIBFT_INCLUDEDIR = includes
LIBFT = $(LIBFTDIR)/libft.a

INCLUDES_NO_PREFIX	=	nm.h otool.h

SOURCES = $(addprefix $(SRCDIR)/, $(SRCS_NO_PREFIX))
DLST_SOURCES = $(addprefix $(SRCDIR)/$(DLSTDIR)/, $(DLST_SRCS_NO_PREFIX))
OBJECTS = $(addprefix $(OBJDIR)/, $(SRCS_NO_PREFIX:%.c=%.o))
DLST_OBJECTS = $(addprefix $(OBJDIR)/$(DLSTDIR)/, $(DLST_SRCS_NO_PREFIX:%.c=%.o))
INCLUDES = $(addprefix $(INCLUDESDIR)/, $(INCLUDES_NO_PREFIX))


INC = -I $(INCLUDESDIR) -I $(LIBFTDIR)/$(LIBFT_INCLUDEDIR)\

CFLAGS = -DPATH=$(PWD) -Wall -Wextra -Werror $(INC)
LFLAGS = -L $(LIBFTDIR)

ifeq ($(DEBUG), 1)
	MFLAGS = debug
	LFLAGS += -fsanitize=address
	CFLAGS += -DDEBUG
	CC += -g3
endif


all: $(LIBFT) $(NM_NAME) $(OTOOL_NAME)

debug:
	make all DEBUG=1

$(NM_NAME): $(NM_DIR)/$(NM_NAME)
	rsync -u $< $@

$(NM_DIR)/$(NM_NAME): $(LIBFT)
	make -C $(NM_DIR) DEBUG=$(DEBUG)

$(OTOOL_NAME): $(OTOOL_DIR)/$(OTOOL_NAME)
	rsync -u $< $@

$(OTOOL_DIR)/$(OTOOL_NAME): $(LIBFT)
	make -C $(OTOOL_DIR) DEBUG=$(DEBUG)

$(LIBFT):
	make -C $(LIBFTDIR) DEBUG=$(DEBUG)

clean:
	make -C $(NM_DIR) clean
	make -C $(OTOOL_DIR) clean

fclean:
	make -C $(NM_DIR) clean
	make -C $(OTOOL_DIR) clean
	rm -rf $(NM_NAME)
	rm -rf $(OTOOL_NAME)

re: fclean
	make

.PHONY: all clean fclean re debug
