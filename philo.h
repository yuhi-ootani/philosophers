/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 15:39:42 by otaniyuhi         #+#    #+#             */
/*   Updated: 2025/02/16 11:09:26 by oyuhi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <errno.h>  //EINVAL EDEADLK
# include <limits.h> // INT_MAX
# include <pthread.h>
// mutex:init destroy lock unlock threads:create join detach
# include <stdbool.h>
# include <stdio.h>    //printf
# include <stdlib.h>   //malloc free
# include <sys/time.h> //gettimeofday
# include <unistd.h>   //write, usleep

/*** DEFINE ***/
# define DEBUG_MODE 0

/*** OPCODE FOR mutex ***/
// enum = a user defined type of named integer identifiers
// 				helps to make a program more readeble
typedef enum e_init_status
{
	INIT_SUCCESS = -1,
	INIT_NOT_YET = 0,
	INIT_FAIL = 1
}						t_init_status;

typedef enum opcode
{
	LOCK,
	UNLOCK,
	DESTROY,
	INIT,
	CREATE,
	E_JOIN,
	DETACH,
}						t_opcode;

// 1s == 1000ms
// 1ms == 1000mus
typedef enum e_unit
{
	SECOND,
	MILLI,
	MICRO,
}						t_unit;

typedef enum e_status
{
	FORK1,
	FORK2,
	EAT,
	SLEEP,
	THINK,
	DIED,
}						t_status;

/***  typedef struct ***/
typedef pthread_mutex_t	t_mtx;
typedef struct s_table	t_table;
typedef struct s_fork	t_fork;
typedef struct s_philo	t_philo;

/*** TABLE STRUCT MEMO ***/
// must_eat_count;      [5] | FLAG if -1
// start_simulation;    timestamp
// threads_running_nbr; used for monitor start
// end_simulation;      a philo dies or all philos full
// threads_ready;       used to start threads simultaneously
// monitor;            used to start threads simultaneously
// t_mutex;        avoid races while reading from table
// w_mutex;        avoid message mixed up
struct					s_table
{
	long				philo_nbr;
	long				time_to_die;
	long				time_to_eat;
	long				time_to_sleep;
	long				must_eat_count;
	long				start_simulation;
	long				threads_running_nbr;
	bool				end_simulation;
	bool				threads_ready;
	pthread_t			monitor;
	t_mtx				t_mutex;
	t_mtx				w_mutex;
	t_fork				*forks;
	t_philo				*philos;
};

struct					s_fork
{
	t_mtx				f_mutex;
	int					fork_id;
};

// last_meal_time; time passed from last meal
struct					s_philo
{
	int					id;
	long				meals_counter;
	bool				full;
	long				last_meal_time;
	t_fork				*first_fork;
	t_fork				*second_fork;
	pthread_t			thread_id;
	t_mtx				p_mutex;
	t_table				*table;
};

/*** PORTOTYPE ***/
// parsing
bool					parse_input(t_table *table, char **av);
// init
bool					data_init(t_table *table);
// dinner
void					dinner(t_table *table, t_philo *philos, int i);
void					wait_all_threads(t_table *table);
// monitor
void					*monitor_dinner(void *arg);
// clean
bool					clean(t_table *table, int w, int f, int p);
// safe
bool					mutex_handle(t_mtx *mutex, t_opcode opcode);
bool					thread_handle(pthread_t *thread, void *(*f)(void *),
							void *arg, t_opcode opcode);
// set_get_mutex
void					set_bool(t_mtx *mutex, bool *dest, bool value);
bool					get_bool(t_mtx *mutex, bool *dest);
void					set_long(t_mtx *mutex, long *dest, long value);
long					get_long(t_mtx *mutex, long *dest);
bool					get_end_simulation(t_table *table);
// time_utils
long					gettime(t_unit unit);
void					precise_usleep(t_table *table, long usec);
// write
bool					write_status(t_philo *philo, t_status status,
							bool DEBUG);

#endif