/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fmt_parse.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stestein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/29 17:36:58 by stestein          #+#    #+#             */
/*   Updated: 2018/06/03 21:13:57 by stestein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftprintf.h"

t_bool		ft_chk_flags(const char **format, t_info *pfinfo)
{
	const char	flags[] = "-+ 0#";
	t_fmt *head;

	head = malloc(sizeof(t_fmt));
	head->str = flags;
	head->i = 0;
	while (head->str[head->i] != **format && head->str[head->i])
		head->i++;
	if (head->str[head->i])
	{
		pfinfo->flags = pfinfo->flags | (1 << head->i);
		(*format)++;
		free(head);
		return (true);
	}
	free(head);
	return (false);
}

t_bool		ft_chk_width(const char **format, t_info *pfinfo, va_list ap)
{
	t_fmt head;

	head.res = 0;
	if (**format == '*')
	{
		pfinfo->width = va_arg(ap, int);
		if (pfinfo->width < 0)
		{
			if (!(pfinfo->flags & LFT))
				pfinfo->flags ^= LFT;
			pfinfo->width = pfinfo->width * -1;
		}
		(*format)++;
		return (true);
	}
	if (!ISDIGIT(**format))
		return (false);
	while (ISDIGIT(**format))
	{
		head.res = head.res * 10 + (**format - '0');
		(*format)++;
	}
	pfinfo->width = head.res;
	return (true);
}

t_bool		ft_chk_prec(const char **format, t_info *pfinfo, va_list ap)
{
	t_fmt head;

	head.res = 0;
	if (**format != '.')
		return (false);
	pfinfo->pset = 1;
	(*format)++;
	if (**format == '*')
	{
		pfinfo->prec = va_arg(ap, int);
		if (pfinfo->prec < -1)
			pfinfo->prec = -1;
		(*format)++;
		return (true);
	}
	while (ISDIGIT(**format))
	{
		head.res = head.res * 10 + (**format - '0');
		(*format)++;
	}
	pfinfo->prec = head.res;
	return (true);
}

t_bool		ft_chk_len(const char **format, t_info *pfinfo)
{
	const char	lengths[] = "hhlljz";
	t_fmt head;
	t_fmt *top;

	top = malloc(sizeof(t_fmt));
	head.str = lengths;
	head.res = 0;
	top->res = 0;
	if (ISLENMOD(**format))
	{
		top->res++;
		head.res = ft_findchr(head.str, **format);
	
		if (head.res == 0 && top->res < 90)
		{
			if (*(*format + 1) == 'h')
				++*format;
			else
				head.res = 1;
		}
		top->res++;
		if (head.res == 2 && top->res != 90)
			head.res = (*(*format + 1) == 'l') ? 3 : 2;
		pfinfo->length = MAX(head.res, pfinfo->length);
		++*format;
		return (true);
	}
	return (false);
}
