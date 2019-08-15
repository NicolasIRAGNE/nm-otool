/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_archive.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/25 18:19:32 by ldedier           #+#    #+#             */
/*   Updated: 2019/08/14 18:25:20 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mach_o.h"

int		process_fill_identifier(t_header_parser *parser,
			t_browser *browser, char *identifier, int *address)
{
	int len;

	len = ft_strlen(identifier);
	if ((off_t)(*address + sizeof(int32_t)) > browser->st.st_size)
	{
		return (-1);
	}
	if (!ft_strncmp((char *)parser->ptr + *address, identifier, len))
	{
		*address = align(*address + len + 1, 4);
		return (0);
	}
	return (1);
}

int		fill_identifier(t_header_parser *parser, t_browser *browser,
			int *address)
{
	int ret;

	if ((ret = process_fill_identifier(parser, browser,
		SYMDEF_SORTED, address)))
	{
		if (ret == -1)
		{
			return (1);
		}
		if ((ret = process_fill_identifier(parser, browser,
			SYMDEF, address)))
		{
			if (ret == -1)
				return (1);
		}
	}
	return (0);
}

int		fill_ranlib(t_header_parser *parser, t_browser *browser,
			struct ranlib ranlib)
{
	//check size;
	return (fill_archive_member(browser, parser, ranlib.ran_off,
		parser->parser_union.ar.stringtable + ranlib.ran_un.ran_strx));
}

int		process_fill_browser_archive(t_header_parser *parser,
			t_browser *browser)
{
	int				ret;
	int				i;

	i = 0;
	while ((uint32_t)i < parser->parser_union.ar.ranlib_array_len)
	{
		if ((ret = fill_ranlib(parser, browser,
			parser->parser_union.ar.ranlib_array[i])))
		{
			if (ret == 1)
				return (ret);
		}
		i++;
	}
	return (0);
}

int		get_ranlib_struct(t_header_parser *parser, t_browser *browser,
			int *address)
{
	parser->parser_union.ar.ranlib_array_size =
		*(uint32_t *)(parser->ptr + *address);
	parser->parser_union.ar.ranlib_array_len =
		*(uint32_t *)(parser->ptr + *address) / sizeof(struct ranlib);
	*address += 4;
	if ((off_t)(*address + parser->parser_union.ar.ranlib_array_len
		* sizeof(struct ranlib)) > browser->st.st_size)
	{
		ft_dprintf(2, "array exceeds file size\n");
		return (1);
	}
	parser->parser_union.ar.ranlib_array =
		(struct ranlib *)((void *)parser->ptr + *address);
	*address += parser->parser_union.ar.ranlib_array_len * sizeof(struct ranlib);
	if ((off_t)(*address + sizeof(uint32_t)) > browser->st.st_size)
	{
		return (1);
	}
	return (0);
}

int		fill_browser_archive(t_header_parser *parser, t_browser *browser)
{
	int				address;
	int				name_size;
	int				ret;

	name_size = ft_atoi(parser->ptr + ft_strlen(ARCHIVE_IDENTIFIER) + 3);
	address = ft_strlen(ARCHIVE_IDENTIFIER) + 60 + name_size;
	if ((ret = get_ranlib_struct(parser, browser, &address)))
		return (ret);
	parser->parser_union.ar.string_array_size = *(uint32_t *)(parser->ptr
		+ address);
	parser->parser_union.ar.stringtable = parser->ptr + address + 4;
	return (process_fill_browser_archive(parser, browser));
}
