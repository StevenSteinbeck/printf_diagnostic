/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_num_conv.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stestein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/29 18:09:55 by stestein          #+#    #+#             */
/*   Updated: 2018/06/03 12:21:19 by stestein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftprintf.h"

# define FLAG_HH return ((signed char)va_arg(ap, int));
# define FLAG_H return ((short)va_arg(ap, int));
# define FLAG_L return (va_arg(ap, long));
# define FLAG_LL return (va_arg(ap, long long));
# define FLAG_J return (va_arg(ap, intmax_t));
# define FLAG_Z return (va_arg(ap, ssize_t));

intmax_t	ft_int_len(char length, va_list ap)
{
	t_num	head;

	head.length = length;
	if (head.length == hh)
		FLAG_HH;
	if (head.length == h)
		FLAG_H;
	if (head.length == l)
		FLAG_L;
	if (head.length == ll)
		FLAG_LL;
	if (head.length == j)
		FLAG_J;
	if (head.length == z)
		FLAG_Z;
	return (va_arg(ap, int));
}

void		ft_num_conv(t_vector *vector, t_info *pfinfo, va_list ap)
{
	t_num	head;
	
	if (pfinfo->spec == 'D')
		pfinfo->length = l;
	if (pfinfo->spec == 'i' || pfinfo->spec == 'D')
		pfinfo->spec = 'd';
	head.val = ft_int_len(pfinfo->length, ap);
	head.s = ft_imaxtoa(head.val);
	if (pfinfo->prec != -1 && pfinfo->flags & ZER)
		pfinfo->flags ^= ZER;
	if (pfinfo->prec == 0 && !ft_strcmp("0", head.s))
		head.s[0] = '\0';
	if (((pfinfo->flags & POS || pfinfo->flags & INV) && head.s[0] != '-')
														&& pfinfo->spec == 'd')
	{
		ft_insrt_to_str(&head.s, (pfinfo->flags & INV) ? " " : "+");
		head.s[0] = ((pfinfo->flags & POS)) ? '+' : head.s[0];
	}
	ft_prec_nums(pfinfo, &head.s);
	ft_pad_handle(pfinfo, &head.s);
	ft_vector_append(vector, head.s);
	free(head.s);
}

void		ft_octal_conv(t_vector *vector, t_info *pfinfo, va_list ap)
{
	t_num	head;

	if (pfinfo->spec == 'O')
		pfinfo->length = l;
	head.octal = ft_xou_len(pfinfo->length, ap);
	head.s = ft_uimaxtoa_base(head.octal, 8, "01234567");
	ft_handle_xou(&head.s, pfinfo);
	ft_vector_append(vector, head.s);
	free(head.s);
}

void		ft_hex_conv(t_vector *vector, t_info *pfinfo, va_list ap)
{
	t_num	head;

	if (pfinfo->spec == 'p')
		pfinfo->length = j;
	head.hex = ft_xou_len(pfinfo->length, ap);
	head.s = ft_uimaxtoa_base(head.hex, 16, "0123456789abcdef");
	if (pfinfo->spec == 'p' && pfinfo->flags & ZER && pfinfo->pset)
		pfinfo->flags ^= ZER;
	ft_handle_xou(&head.s, pfinfo);
	ft_vector_append(vector, head.s);
	free(head.s);
}

void		ft_uns_conv(t_vector *vector, t_info *pfinfo, va_list ap)
{
	t_num	head;

	if (pfinfo->spec == 'U')
		pfinfo->length = l;
	head.unsonian = ft_xou_len(pfinfo->length, ap);
	head.s = ft_uimaxtoa_base(head.unsonian, 10, "0123456789");
	ft_handle_xou(&head.s, pfinfo);
	ft_vector_append(vector, head.s);
	free(head.s);
}