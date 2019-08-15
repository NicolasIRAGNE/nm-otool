/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_symbols.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/15 16:54:59 by ldedier           #+#    #+#             */
/*   Updated: 2019/08/15 16:55:13 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mach_o.h"

char		*get_new_str_from_buffer(char *ptr, size_t n)
{
	char *res;

	if (!(res = (char *)malloc(sizeof(char) * n + 1)))
		return (NULL);
	ft_memcpy(res, ptr, n);
	res[n] = '\0';
	return (res);
}

char		*compute_symbol64_name(t_symbol64 *symbol, char *symbol_name)
{
	if (symbol->bad_index)
	{
		if (symbol->length == -1)
			return (ft_strdup(BAD_INDEX_STR));
		else
			return (get_new_str_from_buffer(symbol_name, symbol->length));
	}
	return (ft_strdup(symbol_name));
}

char		*compute_symbol32_name(t_symbol32 *symbol, char *symbol_name)
{
	if (symbol->bad_index)
	{
		if (symbol->length == -1)
			return (ft_strdup(BAD_INDEX_STR));
		else
			return (get_new_str_from_buffer(symbol_name, symbol->length));
	}
	return (ft_strdup(symbol_name));
}

t_symbol	*nm_new_symbol64(struct nlist_64 *nlist, char *symbol_name,
		int index, t_browser *browser)
{
	t_symbol	*symbol;

	(void)nlist;
	if (!(symbol = (t_symbol *)malloc(sizeof(t_symbol))))
		return (NULL);
	symbol->symbol_union.symbol64.nlist = nlist;
	symbol->symbol_union.symbol64.index = index;
	if ((symbol->symbol_union.symbol64.bad_index =
				is_corrupted_string(symbol_name, browser,
					&symbol->symbol_union.symbol64.length))
			&& symbol->symbol_union.symbol64.length == -1
			&& !browser->has_bad_index)
	{
		fill_bad_index(browser, index);
	}
	if (!(symbol->symbol_union.symbol64.name = compute_symbol64_name(
					&symbol->symbol_union.symbol64, symbol_name)))
	{
		free(symbol);
		return (NULL);
	}
	symbol->symbol_enum = E_SYMBOL_64;
	return (symbol);
}

t_symbol	*nm_new_symbol32(struct nlist *nlist, char *symbol_name,
		int index, t_browser *browser)
{
	t_symbol *symbol;

	if (!(symbol = (t_symbol *)malloc(sizeof(t_symbol))))
		return (NULL);
	symbol->symbol_union.symbol32.nlist = nlist;
	symbol->symbol_union.symbol32.index = index;
	if ((symbol->symbol_union.symbol32.bad_index =
				is_corrupted_string(symbol_name, browser,
					&symbol->symbol_union.symbol32.length))
			&& symbol->symbol_union.symbol32.length == -1)
	{
		browser->has_bad_index = 1;
	}
	if (!(symbol->symbol_union.symbol32.name = compute_symbol32_name(
					&symbol->symbol_union.symbol32, symbol_name)))
	{
		free(symbol);
		return (NULL);
	}
	symbol->symbol_enum = E_SYMBOL_32;
	return (symbol);
}
