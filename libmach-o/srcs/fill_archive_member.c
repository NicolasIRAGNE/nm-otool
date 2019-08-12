/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_archive_member.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/17 11:43:14 by ldedier           #+#    #+#             */
/*   Updated: 2019/08/12 18:17:16 by ldedier          ###   ########.fr       */
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
	int ret;

	(void)j;
	(void)object_file;
	archive_member = parser->ptr + offset;
	if ((off_t)(offset + 60) > browser->st.st_size)
	{
		if (browser->last_member_name)
		{
			ft_dprintf(2, "%s: %s: truncated of marformed archive (offset of "
				"next archive member past the end of the archive after "
					"member %s)\n", browser->progname, browser->filename,
						browser->last_member_name);
		}
		else
		{
			ft_dprintf(2, "%s: %s: truncated of marformed archive (offset of"
				" first archive member past the end of the archive)\n",
					browser->progname, browser->filename);
		}
		return (CORRUPTED);
	}
	if (!ft_strncmp((char *)archive_member, "#1/", 3))
	{
		name_size = ft_atoi((char *)(archive_member + 3));
		if ((off_t)(offset + 60 + name_size) > browser->st.st_size)
		{
			return (CORRUPTED);
		}
		init_parser(&new_parser, (void *)parser->ptr + offset + 60 + name_size,
			offset + 60 + name_size, parser->filename);
		new_parser.parser_enum = PARSER_ENUM_OBJECT;
//		ft_printf("%s\n\n", object_file);
		new_parser.parser_union.object.name = (void *)parser->ptr + offset + 60;
		browser->last_member_name = new_parser.parser_union.object.name;
		new_parser.parser_union.object.ran_off = offset;
//	ft_printf(GREEN"\n");
	//	system("leaks ft_nm");
//		usleep(1000);
		if ((ret = fill_browser(&new_parser, browser, 0)))
		{
		//	free_parser(&new_parser);
			return (ret);
		}
	//	ft_printf("%d\n", j++);
	//	ft_printf("%s\n", new_parser.parser_union.object.name);
		//nm_print_header_parser(new_parser, browser, 2, 1);
	//	ft_printf(RED"\n");
	//	system("leaks ft_nm");
//		usleep(1000);
//		free_parser(parser);
	}
	else
	{
		ft_dprintf(2, "file not well formatted\n");
		return (CORRUPTED);
	}
	return (0);
}
