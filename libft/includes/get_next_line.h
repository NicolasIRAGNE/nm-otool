/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/08 15:42:52 by ldedier           #+#    #+#             */
/*   Updated: 2018/11/15 14:24:58 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_GET_NEXT_LINE_H
# define FT_GET_NEXT_LINE_H

# include <limits.h>
# include <stdlib.h>
# include <unistd.h>
# include "libft.h"
# define BUFF_SIZE 4096

typedef struct	s_gnl
{
	int			fd;
	char		*rest;
	char		*whole_buffer;
}				t_gnl;

int				get_next_line(int const fd, char **line);

#endif
