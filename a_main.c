/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   a_main.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 18:20:31 by otaniyuhi         #+#    #+#             */
/*   Updated: 2025/02/16 11:07:20 by oyuhi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
 * check the number of arguments
 *./philo 5 800 200 200 [5]
 * 1) errors checking, filling table table
 * 2) creating the actual thing
 * 3) start dinner simulation
 * 4) clean everything, when philo die or full
 */
int	main(int ac, char **av)
{
	t_table	table;

	if (ac == 5 || ac == 6)
	{
		if (!parse_input(&table, av))
			return (2);
		if (!data_init(&table))
			return (3);
		dinner(&table, table.philos, -1);
		clean(&table, INIT_SUCCESS, INIT_SUCCESS, INIT_SUCCESS);
	}
	else
	{
		printf("Wrong input.\nUSAGE:./philo 5 800 200 200 [5]");
		return (1);
	}
	return (0);
}
