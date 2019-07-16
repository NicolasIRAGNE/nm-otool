/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_tools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/21 01:35:21 by ldedier           #+#    #+#             */
/*   Updated: 2019/07/16 09:32:47 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

char		*get_new_str_from_buffer(char *ptr, size_t n)
{
	char *res;

	if (!(res = malloc(sizeof(char) * n + 1)))
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
				t_nm_browser *browser)
{
	t_symbol	*symbol;

	if (!(symbol = (t_symbol *)malloc(sizeof(t_symbol))))
		return (NULL);
	symbol->symbol_union.symbol64.nlist = nlist;
	if ((symbol->symbol_union.symbol64.bad_index =
		is_corrupted_string(symbol_name, browser,
			&symbol->symbol_union.symbol64.length))
				&& symbol->symbol_union.symbol64.length == -1)
	{
		browser->has_bad_index = 1;
	}
	if (!(symbol->symbol_union.symbol64.name
		= compute_symbol64_name(&symbol->symbol_union.symbol64, symbol_name)))
	{
		free(symbol);
		return (NULL);
	}
	symbol->symbol_enum = E_SYMBOL_64;
	return (symbol);
}

t_symbol	*nm_new_symbol32(struct nlist *nlist, char *symbol_name,
				t_nm_browser *browser)
{
	t_symbol *symbol;

	if (!(symbol = (t_symbol *)malloc(sizeof(t_symbol))))
		return (NULL);
	symbol->symbol_union.symbol32.nlist = nlist;
	symbol->symbol_union.symbol32.name = symbol_name;
	if ((symbol->symbol_union.symbol32.bad_index =
		is_corrupted_string(symbol_name, browser,
			&symbol->symbol_union.symbol32.length))
				&& symbol->symbol_union.symbol32.length == -1)
	{
		browser->has_bad_index = 1;
	}
	if (!(symbol->symbol_union.symbol32.name
		= compute_symbol32_name(&symbol->symbol_union.symbol32, symbol_name)))
	{
		free(symbol);
		return (NULL);
	}
	symbol->symbol_enum = E_SYMBOL_32;
	return (symbol);
}

int		add_symbol_to_browser(t_header_parser *parser,
			t_nm_browser *browser, t_symbol *new_symbol)
{
	return (ft_tree_add_sorted(&parser->symbols, new_symbol,
				browser, browser->sort_func));
}

uint32_t max_uint32(uint32_t a, uint32_t b)
{
	if (a > b)
		return (a);
	else
		return (b);
}

int nm_perror(char *error_message, t_nm_browser *browser)
{
	ft_dprintf(2, "%s: %s\n", browser->filename, error_message);
	return (0);
}

int		should_add_symbol(uint8_t n_type, uint16_t n_desc, char *name,
			t_nm_browser *browser)
{
	(void)browser;
	(void)name;
	(void)n_desc;
	/*
	if (!(ft_strncmp(name, ".objc", ft_strlen(".objc"))))
	{
		ft_printf(RED);
		ft_printf("name: %s\ntype: %u\ndesc:%d\n\n", name, n_type, GET_LIBRARY_ORDINAL(n_desc));
		ft_printf(EOC);
	}
	else
		ft_printf("name: %s\ntype: %u\ndesc:%d\n\n", name, n_type, GET_LIBRARY_ORDINAL(n_desc));
	*/
	if (n_type & N_STAB )//|| GET_LIBRARY_ORDINAL(n_desc))// || n_desc) & N_DESC_DISCARDED)
		return (0);
	return (1);
}
