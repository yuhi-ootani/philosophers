/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_get_mutex.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otaniyuhi <otaniyuhi@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 11:31:32 by otaniyuhi         #+#    #+#             */
/*   Updated: 2025/02/11 11:32:56 by otaniyuhi        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
 * LOCK and UNLOCK mutex with getting or setting value
 * BOOL AND LONG
 */
void	set_bool(t_mtx *mutex, bool *dest, bool value)
{
	mutex_handle(mutex, LOCK);
	*dest = value;
	mutex_handle(mutex, UNLOCK);
}

bool	get_bool(t_mtx *mutex, bool *dest)
{
	bool	result;

	mutex_handle(mutex, LOCK);
	result = *dest;
	mutex_handle(mutex, UNLOCK);
	return (result);
}

void	set_long(t_mtx *mutex, long *dest, long value)
{
	mutex_handle(mutex, LOCK);
	*dest = value;
	mutex_handle(mutex, UNLOCK);
}

long	get_long(t_mtx *mutex, long *dest)
{
	long	result;

	mutex_handle(mutex, LOCK);
	result = *dest;
	mutex_handle(mutex, UNLOCK);
	return (result);
}

bool	get_end_simulation(t_table *table)
{
	return (get_bool(&table->t_mutex, &table->end_simulation));
}
