/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c_init.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otaniyuhi <otaniyuhi@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 07:51:08 by otaniyuhi         #+#    #+#             */
/*   Updated: 2025/02/11 12:54:31 by otaniyuhi        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
 * To prevent from deadlock.
 * an even philo takes right fork first.
 * an odd philo takes left fork first.
 */
static bool	philos_init(t_table *table)
{
	int		i;
	t_philo	*philo;

	i = -1;
	while (++i < table->philo_nbr)
	{
		philo = &table->philos[i];
		philo->id = i + 1;
		philo->meals_counter = 0;
		philo->full = false;
		philo->table = table;
		if (!mutex_handle(&philo->p_mutex, INIT))
			return (clean(table, INIT_SUCCESS, INIT_SUCCESS, i));
		philo->first_fork = &table->forks[i];
		philo->second_fork = &table->forks[(i + 1) % table->philo_nbr];
		if (i % 2 != 0)
		{
			philo->first_fork = &table->forks[(i + 1) % table->philo_nbr];
			philo->second_fork = &table->forks[i];
		}
	}
	return (true);
}

static bool	forks_init(t_table *table)
{
	int	i;

	i = -1;
	while (++i < table->philo_nbr)
	{
		if (!mutex_handle(&table->forks[i].f_mutex, INIT))
			return (clean(table, INIT_SUCCESS, i, INIT_NOT_YET));
		table->forks[i].fork_id = i;
	}
	return (true);
}

bool	data_init(t_table *table)
{
	table->end_simulation = false;
	table->threads_ready = false;
	table->threads_running_nbr = 0;
	table->philos = (t_philo *)malloc(sizeof(t_philo) * table->philo_nbr);
	if (!table->philos)
		return ((printf("malloc philos failed."), false));
	table->forks = (t_fork *)malloc(sizeof(t_fork) * table->philo_nbr);
	if (!table->forks)
		return ((printf("malloc forks failed."), free(table->philos), false));
	if (!mutex_handle(&table->t_mutex, INIT))
		return ((free(table->philos), free(table->forks), false));
	if (!mutex_handle(&table->w_mutex, INIT))
		return (clean(table, INIT_FAIL, INIT_NOT_YET, INIT_NOT_YET));
	if (!forks_init(table))
		return (false);
	if (!philos_init(table))
		return (false);
	return (true);
}
