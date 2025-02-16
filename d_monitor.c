/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   d_monitor.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 19:58:45 by otaniyuhi         #+#    #+#             */
/*   Updated: 2025/02/16 13:21:32 by oyuhi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
 * check all philos are full
 * increment the count if a philo is full, if not, reset the count
 */
static bool	is_all_philo_full(t_table *table, t_philo *philo)
{
	static int	full_philo_count = 0;

	if (get_bool(&philo->p_mutex, &philo->full))
		full_philo_count++;
	else
		full_philo_count = 0;
	if (full_philo_count == table->philo_nbr)
		return (true);
	return (false);
}

/*
 * check whether philo died
 * last-meal - current_time > time_to_die = DIED!!
 */
static bool	is_philo_died(t_table *table, t_philo *philo)
{
	long	elapsed;
	long	t_to_die;
	long	last_meal_time;

	last_meal_time = get_long(&philo->p_mutex, &philo->last_meal_time);
	elapsed = gettime(MILLI) - last_meal_time;
	t_to_die = table->time_to_die / 1e3;
	if (elapsed > t_to_die)
		return (true);
	return (false);
}

/*
 * spinlock till all threads are running
 * itelare every philo to check if philo died till died or full.
 */
void	*monitor_dinner(void *arg)
{
	t_table	*table;
	int		i;

	table = (t_table *)arg;
	while (!(get_long(&table->t_mutex,
				&table->threads_running_nbr) == table->philo_nbr))
		;
	while (!get_end_simulation(table))
	{
		i = -1;
		while (++i < table->philo_nbr && !get_end_simulation(table))
		{
			if (is_all_philo_full(table, &table->philos[i]))
				set_bool(&table->t_mutex, &table->end_simulation, true);
			if (is_philo_died(table, &table->philos[i]))
			{
				set_bool(&table->t_mutex, &table->end_simulation, true);
				write_status(&table->philos[i], DIED, DEBUG_MODE);
			}
		}
	}
	return (NULL);
}
