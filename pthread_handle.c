/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pthread_handle.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otaniyuhi <otaniyuhi@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 07:51:11 by otaniyuhi         #+#    #+#             */
/*   Updated: 2025/02/10 12:21:50 by otaniyuhi        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	handle_error(int status, t_opcode opcode)
{
	if (status == 0)
		return (true);
	else if (status == EINVAL && (opcode >= LOCK && opcode <= DESTROY))
		printf("The value specified by mutex is invalid.");
	else if (status == EINVAL && (opcode == INIT || opcode == CREATE))
		printf("The value specified by attr is invalid.");
	else if (status == EINVAL && (opcode == E_JOIN || opcode == DETACH))
		printf("the specified thread does not refer to a joinable thread.");
	else if (status == EDEADLK && opcode == LOCK)
		printf("A deadlock would occur.");
	else if (status == EPERM && opcode == UNLOCK)
		printf("The current thread does not hold a lock on mutex.");
	else if (status == EBUSY && opcode == DESTROY)
		printf("Mutex is locked.");
	else if (status == ENOMEM && opcode == INIT)
		printf("Not enough memory to create another mutex.");
	else if (status == EAGAIN && opcode == CREATE)
		printf("Lacked the necessary resources to create another thread");
	else if (status == EPERM && opcode == CREATE)
		printf("The caller does not have appropriate permission");
	else if (status == EDEADLK && opcode == E_JOIN)
		printf("Deadlock or the value of thread specifies the calling thread.");
	else if (status == ESRCH && (opcode == E_JOIN || opcode == DETACH))
		printf("No thread found for the given thread ID.");
	return (false);
}

// *** MUTEX SAFE *** //
/*
 *init  // if fail, abort the program
 * the second arrgument of mutex init is for attributes. (NULL means default)
 *destroy //  // if fail, continue
 *lock // if fail, continue
 *unlock // if fail, countinue
 */
bool	mutex_handle(t_mtx *mutex, t_opcode opcode)
{
	if (opcode == LOCK)
		return (handle_error(pthread_mutex_lock(mutex), opcode));
	else if (opcode == UNLOCK)
		return (handle_error(pthread_mutex_unlock(mutex), opcode));
	else if (opcode == INIT)
		return (handle_error(pthread_mutex_init(mutex, NULL), opcode));
	else if (opcode == DESTROY)
		return (handle_error(pthread_mutex_destroy(mutex), opcode));
	else
		return ((printf("Mutex's opcode is wrong!"), false));
}

// *** MUTEX SAFE *** //
/*
 *create //if fail, abort the program 
 * If attr is NULL, the default attributes are used.
 *join // if fail, abort the program.
       :wait for thread termination
 *detach //do not use
 */
bool	thread_handle(pthread_t *thread, void *(*f)(void *), void *arg,
		t_opcode opcode)
{
	if (opcode == CREATE)
		return (handle_error(pthread_create(thread, NULL, f, arg), opcode));
	else if (opcode == E_JOIN)
		return (handle_error(pthread_join(*thread, NULL), opcode));
	else if (opcode == DETACH)
		return (handle_error(pthread_detach(*thread), opcode));
	else
		return ((printf("Thread's opcode is wrong!"), false));
}
