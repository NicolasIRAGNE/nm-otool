/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_otool.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/18 11:26:01 by ldedier           #+#    #+#             */
/*   Updated: 2019/08/08 19:05:17 by niragne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_OTOOL_H
# define FT_OTOOL_H

# include "mach_o.h"
# include "libft.h"

typedef struct		s_text_printer
{
	unsigned char	*data;
	uint64_t		data_size;
	uint64_t		addr;
	int				should_swap;
	int				alignment;
}					t_text_printer;

typedef struct		s_otool_flags
{
	int			valid : 1;
	int			flag_h : 1;
	int			flag_t : 1;
	int			flag_d : 1;
}					t_otool_flags;


/*
** print.c
*/
void	otool_print(t_browser *browser, t_otool_flags *flags);
int		is_little_endian(void);

/*
** print_sections.c
*/
void	print_parser_header_intro(t_header_parser *parser);
void    otool_process_print_header_parser(t_header_parser *parser,
			cpu_type_t cputype, t_section *section);
/*
** flags
*/
void	flag_h(t_arg_parser *parser, void *flags);
void	flag_t(t_arg_parser *parser, void *flags);
void	flag_d(t_arg_parser *parser, void *flags);
void	flag_invalid(t_arg_parser *parser, void *flags);


#endif
