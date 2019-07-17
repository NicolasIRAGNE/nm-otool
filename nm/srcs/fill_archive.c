/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_archive.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/25 18:19:32 by ldedier           #+#    #+#             */
/*   Updated: 2019/07/17 19:13:03 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int		process_fill_identifier(t_header_parser *parser,
			t_nm_browser *browser, char *identifier, int *address)
{
	int len;

	len = ft_strlen(identifier);
	if ((off_t)(*address + sizeof(int32_t)) > browser->st.st_size)
	{
		return (-1);
	}
	if (!ft_strncmp((char *)parser->ptr + *address, identifier, len))
	{
	//	ft_printf("%d\n", *address);
	//	ft_printf("%d\n", *address + len);
		*address = align(*address + len + 1, 4);
		return (0);
	}
	return (1);
}

int		fill_identifier(t_header_parser *parser, t_nm_browser *browser,
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

void	print_ranlib(t_header_parser *parser, t_nm_browser *browser,
			struct ranlib ranlib, char *stringtable, int strsize)
{
	(void)strsize;
	(void)parser;
	(void)stringtable;
//	ft_printf("library member offset: %d\n", ranlib.ran_off);
//	ft_printf("%s\n", parser->ptr + ranlib.ran_off);
//	ft_printf("%s\n", stringtable + ranlib.ran_un.ran_strx);
	fill_archive_member(browser, parser, ranlib.ran_off,
		stringtable + ranlib.ran_un.ran_strx);
//	ft_printf("%s\n", stringtable + ranlib.ran_off);
}

int		fill_browser_archive(t_header_parser *parser, t_nm_browser *browser)
{
	int				address;
	uint32_t		ranlib_array_len;
	uint32_t		string_array_size;
	struct ranlib	*ranlib_array;
	char			*stringtable;
	int name_size;

	name_size = ft_atoi(parser->ptr + ft_strlen(ARCHIVE_IDENTIFIER) + 3);
	address = ft_strlen(ARCHIVE_IDENTIFIER) + 60;
//	if (fill_identifier(parser, browser, &address))
//	{
//		return (1);
//	}
	address = ft_strlen(ARCHIVE_IDENTIFIER) + 60 + name_size;
	ranlib_array_len = *(uint32_t *)(parser->ptr + address) /
		sizeof(struct ranlib);
	address += 4;
	if ((off_t)(address + ranlib_array_len * sizeof(struct ranlib))
		> browser->st.st_size)
	{
		ft_dprintf(2, "array exceeds file size\n");
		return (1);
	}
	ranlib_array = (struct ranlib *)((void *)parser->ptr + address);
	address += ranlib_array_len * sizeof(struct ranlib);
	if ((off_t)(address + sizeof(uint32_t)) > browser->st.st_size)
	{
		return (1);
	}
//	ft_printf("%s\n", parser->ptr);
	string_array_size = *(uint32_t *)(parser->ptr + address);
	address += sizeof(uint32_t);
   	stringtable = parser->ptr + address;
	int i;
	i = 0;
	while ((uint32_t)i < ranlib_array_len )
	{
		print_ranlib(parser, browser, ranlib_array[i], stringtable, string_array_size);
		i++;
	}
	return (0);
}