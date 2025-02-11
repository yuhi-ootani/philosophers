/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otaniyuhi <otaniyuhi@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 10:15:44 by otaniyuhi         #+#    #+#             */
/*   Updated: 2025/02/11 11:41:40 by otaniyuhi        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
 * eat routine
 * 1) grab the forks
 *  when mutex is locked, the program stops.
 * 2) eat : upadate last meal.
 *          upadate meals counter
 *          write eat
 *          stop for eating time
 * 					eventually bool full
 * 3) release the forks
 */
static void	eat_sleep_think(t_philo *philo, t_table *table)
{
	mutex_handle(&philo->first_fork->f_mutex, LOCK);
	write_status(philo, FORK1, DEBUG_MODE);
	mutex_handle(&philo->second_fork->f_mutex, LOCK);
	write_status(philo, FORK2, DEBUG_MODE);
	set_long(&philo->p_mutex, &philo->last_meal_time, gettime(MILLI));
	philo->meals_counter++;
	write_status(philo, EAT, DEBUG_MODE);
	precise_usleep(table, table->time_to_eat);
	if (table->must_eat_count > 0
		&& (philo->meals_counter == table->must_eat_count))
		set_bool(&philo->p_mutex, &philo->full, true);
	mutex_handle(&philo->second_fork->f_mutex, UNLOCK);
	mutex_handle(&philo->first_fork->f_mutex, UNLOCK);
	write_status(philo, SLEEP, DEBUG_MODE);
	precise_usleep(table, table->time_to_sleep);
	write_status(philo, THINK, DEBUG_MODE);
	if (table->philo_nbr % 2 != 0)
		precise_usleep(table, 30000);
}

/*
 * 1) wait all threads are created to start simultaneously
 * 2) set last meal time as start of simulation
 * 3) increace running threads for monitor thread
 *    why need = monitor should start after simulation time set.
 * 4) while end_simulation false and not full
 * 4-1) eat
 * 4-2) sleep
 * 4-3) think : 30 ms after eating
 */
void	*simulation(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (!get_bool(&philo->table->t_mutex, &philo->table->threads_ready))
		;
	set_long(&philo->p_mutex, &philo->last_meal_time, gettime(MILLI));
	mutex_handle(&philo->table->t_mutex, LOCK);
	philo->table->threads_running_nbr++;
	mutex_handle(&philo->table->t_mutex, UNLOCK);
	if (philo->table->philo_nbr == 1)
	{
		write_status(philo, FORK1, DEBUG_MODE);
		while (!get_end_simulation(philo->table))
			usleep(200);
		return (NULL);
	}
	while (!get_end_simulation(philo->table) && !get_bool(&philo->p_mutex,
			&philo->full))
		eat_sleep_think(philo, philo->table);
	return (NULL);
}
// set_long(&philo->table->t_mutex, &philo->table->threads_running_nbr,
// 	get_long(&philo->table->t_mutex, &philo->table->threads_running_nbr)
// 	+ 1);

/*
 *
 * 0) If no meals, return
 * 1) Create all threads, all philos
 * 2) Create a monitor thread to check death
 * 3) Synchronize the beggining of the simulation
 * 3-1) sets start of simulatiom time
 * 3-2) After all threads are ready! every philo starts simultaneously
 * 4) JOIN everyone
 * 4-1) set end simulation true before JOIN monitor to end code properly.
 *      Because monitor only ends when end_simulation is true.
 */
void	dinner(t_table *table, t_philo *philos, int i)
{
	if (0 == table->must_eat_count)
		return ;
	while (++i < table->philo_nbr)
	{
		if (!thread_handle(&philos[i].thread_id, simulation, &philos[i],
				CREATE))
			return ;
	}
	table->start_simulation = gettime(MILLI);
	if (!thread_handle(&table->monitor, monitor_dinner, table, CREATE))
		return ;
	set_bool(&table->t_mutex, &table->threads_ready, true);
	i = -1;
	while (++i < table->philo_nbr)
	{
		if (!thread_handle(&philos[i].thread_id, NULL, NULL, E_JOIN))
			return ;
	}
	set_bool(&table->t_mutex, &table->end_simulation, true);
	thread_handle(&table->monitor, NULL, NULL, E_JOIN);
}

/***  SPINLOCK TO Synchronize philos start ***/
// void	wait_all_threads(t_table *table)
// {
// 	if (!table)
// 	{
// 		fprintf(stderr, "Error: table pointer is NULL\n");
// 		return ;
// 	}
// 	while (!get_bool(&table->t_mutex, &table->threads_ready))
// 		;
// }

/*** SOLO_SIMULATION IN THE DINNER FUNCTION ***/
// else if (table->philo_nbr == 1 && !thread_handle(&philos[0].thread_id,
// 		lone_simulation, &philos[0], CREATE))
// 	return ;
// while (table->philo_nbr > 1 && ++i < table->philo_nbr)

/*** SOLO_SIMULATION FUNCTION ***/
/*
 * 1) fake to lock a fork
 * 2) Sleep untill the monitor will bust it
 */
// void	*lone_simulation(void *arg)
// {
// 	t_philo	*philo;

// 	philo = (t_philo *)arg;
// 	// wait_all_threads(philo->table);
// 	while (!get_bool(&philo->table->t_mutex, &philo->table->threads_ready))
// 		;
// 	set_long(&philo->p_mutex, &philo->last_meal_time, gettime(MILLI));
// 	mutex_handle(&philo->table->t_mutex, LOCK);
// 	philo->table->threads_running_nbr++;
// 	mutex_handle(&philo->table->t_mutex, UNLOCK);
// 	mutex_handle(&philo->first_fork->fork, LOCK);
// 	write_status(philo, FORK1, DEBUG_MODE);
// 	while (!get_end_simulation(philo->table))
// 		usleep(200);
// 	mutex_handle(&philo->first_fork->fork, UNLOCK);
// 	return (NULL);
// }