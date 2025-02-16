/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_parsing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 18:59:14 by otaniyuhi         #+#    #+#             */
/*   Updated: 2025/02/16 12:22:52 by oyuhi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
 * inline function
 * An inline function is a function that is expanded in line when it is called
 */
static inline bool	is_digit(char c)
{
	return (c >= '0' && c <= '9');
}

static inline bool	is_space(char c)
{
	return ((c >= 9 && c <= 13) || 32 == c);
}

/*
 * VALID_INPUT
 * 1) check for negatives
 * 2) check the fist char is a number or +
 * 3) check for INT_MAX (length is not greater than 10)
 * 3) check input does not contain any other charactors after numbers.
 */
static const char	*valid_input(const char *str)
{
	int			len;
	const char	*number;

	len = 0;
	while (is_space(*str))
		str++;
	if (*str == '+')
		++str;
	if (*str == '-')
		return ((printf("Inputs must be positive value!\n"), NULL));
	if (!is_digit(*str))
		return ((printf("Inputs must contain only digit!\n"), NULL));
	if (*str == '0')
		return ((printf("Inputs must not start from 0!(MORE THAN 0)\n"), NULL));
	number = str;
	while (is_digit(str[len]))
		len++;
	if (len > 10)
		return ((printf("Inputs must be less than INT_MAX!\n"), NULL));
	while (str[len])
	{
		if (!is_space(str[len++]))
			return ((printf("Inputs must be finish with a digit!\n"), NULL));
	}
	return (number);
}

static long	ft_atol(const char *str)
{
	long	num;

	num = 0;
	str = valid_input(str);
	if (!str)
		return (-1);
	while (is_digit(*str))
	{
		num = num * 10 + *str - '0';
		if (num > INT_MAX)
			return ((printf("Value is bigger than INT_MAX!"), -1));
		str++;
	}
	return (num);
}

/*
 *./philo 5 800 200 200 [5]
 * av[1] av[2] av[3] ...
 * 1) actual numbers ✅
 * 2) not > INT_MAX
 * 3) timestamps > 60ms
 * USLEEP function want usec
 */
bool	parse_input(t_table *table, char **av)
{
	table->philo_nbr = ft_atol(av[1]);
	if (table->philo_nbr < 0)
		return (false);
	table->time_to_die = ft_atol(av[2]) * 1e3;
	if (table->time_to_die < 0)
		return (false);
	table->time_to_eat = ft_atol(av[3]) * 1e3;
	if (table->time_to_eat < 0)
		return (false);
	table->time_to_sleep = ft_atol(av[4]) * 1e3;
	if (table->time_to_sleep < 0)
		return (false);
	if (av[5])
	{
		table->must_eat_count = ft_atol(av[5]);
		if (table->must_eat_count < 0)
			return (false);
	}
	else
		table->must_eat_count = -1;
	return (true);
}
