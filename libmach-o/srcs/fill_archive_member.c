/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_archive_member.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/17 11:43:14 by ldedier           #+#    #+#             */
/*   Updated: 2019/07/18 09:45:52 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mach_o.h"
int		fill_archive_member(t_browser *browser,
		t_header_parser *parser, uint32_t offset, char *object_file)
{
	void				*archive_member;
	int					name_size;
	t_header_parser		new_parser;
	static int j = 0;

	(void)j;
	(void)object_file;
	archive_member = parser->ptr + offset;
	if ((off_t)(offset + 60) > browser->st.st_size)
	{
		return (1);
	}
	if (!ft_strncmp((char *)archive_member, "#1/", 3))
	{
		name_size = ft_atoi((char *)(archive_member + 3));
		if ((off_t)(offset + 60 + name_size) > browser->st.st_size)
		{
			return (1);
		}
		init_parser(&new_parser, (void *)parser->ptr + offset + 60 + name_size,
			offset + 60 + name_size, parser->filename);
		new_parser.parser_enum = PARSER_ENUM_OBJECT;
//		ft_printf("%s\n", parser->ptr + offset + 60);
//		ft_printf("%s\n\n", object_file);
		new_parser.parser_union.object.name = (void *)parser->ptr + offset + 60;
		new_parser.parser_union.object.ran_off = offset;
//	ft_printf(GREEN"\n");
	//	system("leaks ft_nm");
//		usleep(1000);
	if (fill_browser(&new_parser, browser) == CORRUPTED)
		{
			free_parser(parser);
			return (0);
		}
	//	ft_printf("%d\n", j++);
	//	ft_printf("%s\n", new_parser.parser_union.object.name);
		//nm_print_header_parser(new_parser, browser, 2, 1);
	//	ft_printf(RED"\n");
	//	system("leaks ft_nm");
//		usleep(1000);
		free_parser(parser);
	}
	else
	{
		ft_dprintf(2, "file not well formatted\n");
		return (1);
	}
	return (0);
}
