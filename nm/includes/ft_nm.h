/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/19 12:58:24 by niragne           #+#    #+#             */
/*   Updated: 2019/08/08 19:12:40 by niragne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NM
# define FT_NM

# include "libft.h"
# include "mach_o.h"

# define DEFAULT_NM_FILE	"a.out"

typedef struct		s_nm_flags
{
	int			valid : 1;
	int			flag_n : 1;
	int			flag_p : 1;
	int			flag_r : 1;
}					t_nm_flags;

typedef	struct 		s_nm_wrapper
{
	t_browser		*browser;
	t_nm_flags		*flags;
}					t_nm_wrapper;

/*
** options_tools.c
*/
void						nm_option_error(char *str, t_browser *browser);
int							describe_option(char *str);
void						print_help(void);

/*
** options_func.c
*/
void						nm_opt_n(t_arg_parser *parser, void *flags);
void						nm_opt_p(t_arg_parser *parser, void *flags);
void						nm_opt_r(t_arg_parser *parser, void *flags);
void						flag_invalid(t_arg_parser *parser, void *flags);
/*
** options.c
*/
int							parse_options(int *i, int ac,
								char **av, t_browser *browser);
/*
** print.c
*/
void						nm_print(t_browser *browser, int force);
#endif
