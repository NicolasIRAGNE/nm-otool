/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sections.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/21 06:06:04 by ldedier           #+#    #+#             */
/*   Updated: 2019/08/16 15:51:31 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SECTIONS_H
# define SECTIONS_H

typedef enum				e_section_enum
{
	E_SECTION_64,
	E_SECTION_32
}							t_section_enum;

typedef union				u_section_union
{
	struct section_64		*section64;
	struct section			*section32;
}							t_section_union;

typedef struct				s_section
{
	t_section_union			section_union;
	t_section_enum			section_enum;
}							t_section;

typedef struct				s_section_arr
{
	t_section				*sections;
	int						size;
}							t_section_arr;

char						*get_section_sectname(t_section *section);
char						*get_section_segname(t_section *section);
uint64_t					get_section_addr(t_section *section);
uint64_t					get_section_size(t_section *section);
uint64_t					get_section_offset(t_section *section);

#endif
