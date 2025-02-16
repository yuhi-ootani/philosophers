/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_status.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 15:49:51 by otaniyuhi         #+#    #+#             */
/*   Updated: 2025/02/16 15:07:49 by oyuhi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	write_status_debug(t_philo *philo, t_status status)
{
	long	elapsed;

	elapsed = gettime(MILLI) - philo->table->start_simulation;
	if (status == FORK1 && !get_end_simulation(philo->table))
		printf("time:%-6ld\t%d has taken a FIRST FORK.\t\t\t[fork id:%d]\n",
			elapsed, philo->id, philo->first_fork->fork_id);
	else if (status == FORK2 && !get_end_simulation(philo->table))
		printf("time:%-6ld\t%d has taken a SECOND FORK.\t\t\t[fork id:%d]\n",
			elapsed, philo->id, philo->second_fork->fork_id);
	else if (status == EAT && !get_end_simulation(philo->table))
		printf("time:%-6ld\t%d is EATING.\t\t\t[%ld times]\n", elapsed,
			philo->id, philo->meals_counter);
	else if (status == SLEEP && !get_end_simulation(philo->table))
		printf("time:%-6ld\t%d is SLEEPING.\n", elapsed, philo->id);
	else if (status == THINK && !get_end_simulation(philo->table))
		printf("time:%-6ld\t%d is THINKING.\n", elapsed, philo->id);
	else if (status == DIED && !get_end_simulation(philo->table))
		printf("time:%-6ld\t%d DIED!💀💀\n", elapsed, philo->id);
}

/*
 * if full doesn't write anything
 * mutex avoid to mix messages up
 * write status following by status enum
 */
void	write_status(t_philo *philo, t_status status, bool DEBUG)
{
	long	elapsed;

	mutex_handle(&philo->table->w_mutex, LOCK);
	if (DEBUG)
		write_status_debug(philo, status);
	else
	{
		elapsed = gettime(MILLI) - philo->table->start_simulation;
		if ((status == FORK1 || status == FORK2)
			&& !get_end_simulation(philo->table))
			printf("%-6ld %d has taken a fork\n", elapsed, philo->id);
		else if (status == EAT && !get_end_simulation(philo->table))
			printf("%-6ld %d is eating\n", elapsed, philo->id);
		else if (status == SLEEP && !get_end_simulation(philo->table))
			printf("%-6ld %d is sleeping\n", elapsed, philo->id);
		else if (status == THINK && !get_end_simulation(philo->table))
			printf("%-6ld %d is thinking\n", elapsed, philo->id);
		else if (status == DIED)
			printf("%-6ld %d died\n", elapsed, philo->id);
	}
	mutex_handle(&philo->table->w_mutex, UNLOCK);
	return ;
}

// bool	write_status(t_philo *philo, t_status status, bool DEBUG)
// {
// 	long	elapsed;

// 	if ((status == DIED) && get_bool(&philo->p_mutex, &philo->full))
// 		return (true);
// 	mutex_handle(&philo->table->w_mutex, LOCK);
// 	if (DEBUG)
// 		write_status_debug(philo, status);
// 	else
// 	{
// 		elapsed = gettime(MILLI) - philo->table->start_simulation;
// 		if ((status == FORK1 || status == FORK2)
// 			&& !get_end_simulation(philo->table))
// 			printf("%-6ld %d has taken a fork\n", elapsed, philo->id);
// 		else if (status == EAT && !get_end_simulation(philo->table))
// 			printf("%-6ld %d is eating\n", elapsed, philo->id);
// 		else if (status == SLEEP && !get_end_simulation(philo->table))
// 			printf("%-6ld %d is sleeping\n", elapsed, philo->id);
// 		else if (status == THINK && !get_end_simulation(philo->table))
// 			printf("%-6ld %d is thinking\n", elapsed, philo->id);
// 		else if (status == DIED)
// 			printf("%-6ld %d died\n", elapsed, philo->id);
// 	}
// 	mutex_handle(&philo->table->w_mutex, UNLOCK);
// 	return (true);
// }
