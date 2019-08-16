/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_archive_member.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/17 11:43:14 by ldedier           #+#    #+#             */
/*   Updated: 2019/08/16 16:05:35 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mach_o.h"

int			is_corrupted_archive_member(int offset, t_browser *browser)
{
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
		return (1);
	}
	return (0);
}

void		init_parser_from_archive_member(t_header_parser *new_parser,
				uint32_t offset, int name_size, t_header_parser *parser)
{
	init_parser(new_parser, (void *)parser->ptr + offset + 60 + name_size,
			offset + 60 + name_size, parser->filename);
	new_parser->parser_enum = PARSER_ENUM_OBJECT;
	new_parser->parser_union.object.name = (void *)parser->ptr + offset + 60;
	new_parser->parser_union.object.ran_off = offset;
}

int			fill_archive_member(t_browser *browser,
		t_header_parser *parser, uint32_t offset, char *object_file)
{
	void				*archive_member;
	int					name_size;
	t_header_parser		new_parser;
	int					ret;

	(void)object_file;
	archive_member = parser->ptr + offset;
	if (is_corrupted_archive_member(offset, browser))
		return (ERROR_CORRUPTED);
	if (!ft_strncmp((char *)archive_member, "#1/", 3))
	{
		name_size = ft_atoi((char *)(archive_member + 3));
		if ((off_t)(offset + 60 + name_size) > browser->st.st_size)
			return (ERROR_CORRUPTED);
		init_parser_from_archive_member(&new_parser, offset, name_size, parser);
		browser->last_member_name = new_parser.parser_union.object.name;
		if ((ret = fill_browser(&new_parser, browser, 0)))
			return (ret);
	}
	else
	{
		ft_dprintf(2, "file not well formatted\n");
		return (ERROR_CORRUPTED);
	}
	return (0);
}
