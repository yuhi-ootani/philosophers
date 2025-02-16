/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otaniyuhi <otaniyuhi@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 18:26:10 by otaniyuhi         #+#    #+#             */
/*   Updated: 2025/02/11 10:44:31 by otaniyuhi        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
 * getttimeofday
 * The first argument (tv) is used to get
 * the current time in seconds and microseconds.
 * The second argument (tz) is obsolete and should typically be passed as NULL.
 * struct timeval {
 *            time_t       tv_sec;   seconds since Jan. 1, 1970
 *           suseconds_t  tv_usec;   and microseconds
 * };
 */
long	gettime(t_unit unit)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		return (-1);
	if (unit == SECOND)
		return (tv.tv_sec + (tv.tv_usec / 1e6));
	else if (unit == MILLI)
		return ((tv.tv_sec * 1e3) + (tv.tv_usec / 1e3));
	else if (unit == MICRO)
		return ((tv.tv_sec * 1e6) + (tv.tv_usec));
	printf("unit is wrong!");
	return (-1);
}

/*
 * precise usleep, the real one suck
 * usleep isn't always precise because:
 *
 *OS Scheduling – The CPU might switch to other tasks
									before resuming delaying execution.
 *Timer Resolution ~ The system clock may not support
										exact microsecond precision.
 *System Load – If the CPU is busy, the sleep duration
								may extend beyond the requested time.
 * SOLUTION *
 *Fixed small sleep interval to prevent CPU overload
 * Sleep for 500 µs or the remaining time if less than 500 µs.
 */
void	precise_usleep(t_table *table, long usec)
{
	long	start;
	long	elapsed;
	long	remaining;

	start = gettime(MICRO);
	elapsed = gettime(MICRO) - start;
	while (elapsed < usec)
	{
		if (get_end_simulation(table))
			break ;
		remaining = usec - elapsed;
		if (remaining > 500)
			usleep(500);
		else
			usleep(remaining);
		elapsed = gettime(MICRO) - start;
	}
}
