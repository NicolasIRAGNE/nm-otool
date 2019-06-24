/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/24 01:32:45 by ldedier           #+#    #+#             */
/*   Updated: 2019/06/24 01:32:45 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void	free_parser(t_header_parser *parser)
{
	ft_tree_del_value(&parser->symbols);
	if (parser->section_arr.sections)
		free(parser->section_arr.sections);
}

void	free_parser_lst(void *content, size_t dummy)
{
	(void)dummy;
	free_parser((t_header_parser *)content);
	free(content);
}
