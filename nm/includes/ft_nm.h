/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/19 12:58:24 by niragne           #+#    #+#             */
/*   Updated: 2019/07/18 12:05:09 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NM
# define FT_NM

# include "libft.h"
# include "mach_o.h"

# define DEFAULT_NM_FILE	"a.out"

/*
** options_tools.c
*/
void						nm_option_error(char *str, t_browser *browser);
int							describe_option(char *str);
void						print_help(void);

/*
** options_func.c
*/
void						nm_opt_n(t_browser *browser, int *options);
void						nm_opt_p(t_browser *browser, int *options);
void						nm_opt_r(t_browser *browser, int *options);
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
