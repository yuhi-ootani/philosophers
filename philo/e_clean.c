/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_clean.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otaniyuhi <otaniyuhi@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 12:00:32 by otaniyuhi         #+#    #+#             */
/*   Updated: 2025/02/11 13:00:39 by otaniyuhi        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	clean_forks(t_fork *forks, int count)
{
	int	i;

	i = -1;
	while (++i < count)
		mutex_handle(&forks[i].f_mutex, DESTROY);
}

static void	clean_philos(t_philo *philos, int count)
{
	int	i;

	i = -1;
	while (++i < count)
		mutex_handle(&philos[i].p_mutex, DESTROY);
}

/*
 * free and destroy every mutex with 4 conditions.
 * all success : all -1
 * p_mtx_nbr : philo mutex failed = destroy till p_mtx_nbr
 * f_mtx_nbr : forks mutex failed = destroy till f_mtx_nbr
 * write_mtx : write mutex failed = INIT_FAIL = 1
 */
bool	clean(t_table *table, int write_mtx, int f_mtx_nbr, int p_mtx_nbr)
{
	if (p_mtx_nbr == INIT_SUCCESS)
		clean_philos(table->philos, table->philo_nbr);
	else if (p_mtx_nbr != INIT_NOT_YET)
		clean_philos(table->philos, p_mtx_nbr);
	if (f_mtx_nbr == INIT_SUCCESS)
		clean_forks(table->forks, table->philo_nbr);
	else if (f_mtx_nbr != INIT_NOT_YET)
		clean_forks(table->forks, f_mtx_nbr);
	if (write_mtx == INIT_SUCCESS)
		mutex_handle(&table->w_mutex, DESTROY);
	mutex_handle(&table->t_mutex, DESTROY);
	free(table->forks);
	free(table->philos);
	return (false);
}
