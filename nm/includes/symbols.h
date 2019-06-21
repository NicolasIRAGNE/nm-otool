/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   symbols.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/20 21:11:46 by ldedier           #+#    #+#             */
/*   Updated: 2019/06/20 21:11:46 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYMBOLS_H
# define SYMBOLS_H

# define SORT_NUMERICAL			1
# define SORT_NONE				2
# define SORT_REVERSE			4

typedef enum				e_symbol_enum
{
	E_SYMBOL_64,
	E_SYMBOL_32
}							t_symbol_enum;

typedef struct				s_symbol64
{
	struct nlist_64			*nlist;
	char					*name;
}							t_symbol64;

typedef struct				s_symbol32
{
	struct nlist			*nlist;
	char					*name;
}							t_symbol32;

typedef union				u_symbol_union
{
	t_symbol64				symbol64;
	t_symbol32				symbol32;
}							t_symbol_union;

typedef struct				s_symbol
{
	t_symbol_union			symbol_union;
	t_symbol_enum			symbol_enum;
	char					debug;
}							t_symbol;

/*
** symbols.c
*/
char						*get_symbol_name(t_symbol *symbol);
uint64_t					get_symbol_value(t_symbol *symbol);

/*
** symbols_sort.c
*/
long						cmp_symbol_alpha(void *s1, void *s2);
long						cmp_symbol_numerical(void *s1, void *s2);
long						cmp_symbol_none(void *s1, void *s2);

#endif
