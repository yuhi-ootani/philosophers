# philosophers

## Summary  
This project solves the Dining Philosophers problem using **pthreads** and **mutexes**, as specified in the 42 School subject. Philosophers alternately think, pick up two forks (mutexes), eat, sleep, and repeat. A philosopher “dies” if they don’t start eating within `time_to_die` ms. Optionally, each must eat `meals_required` times before the simulation ends. :contentReference[oaicite:13]{index=13}

## Structure  
- **Makefile**:  
  - `all`: compiles `*.c` into `philo` with `cc -Wall -Wextra -Werror`.  
  - `clean`: removes `*.o`.  
  - `fclean`: removes `philo` and `*.o`.  
  - `re`: runs `fclean` then `all`. :contentReference[oaicite:14]{index=14}  

- **philo.h**:  
  - Defines `t_params` (shared data: philosopher count, timings, mutex arrays, start timestamp, stop flag) and `t_philo` (ID, last meal time, meals eaten). Declares routines. :contentReference[oaicite:15]{index=15}  

- **main.c**:  
  - Parses arguments (`4` or `5` positives).  
  - Allocates `t_params` and `t_philo` arrays.  
  - Initializes mutexes (`forks[]`, `print_mutex`).  
  - Creates philosopher threads (`philosopher_routine`) and monitor threads (`monitor_routine`).  
  - Joins threads and cleans up. :contentReference[oaicite:16]{index=16}  

- **philosopher.c**:  
  - In a loop (until `params->stop` or `meals_required` reached):  
    - Think → lock two forks (lower index first) → print “has taken a fork” twice → update `last_meal`, print “is eating”, sleep `time_to_eat` → unlock forks → increment meals → print “is sleeping”, sleep `time_to_sleep`. :contentReference[oaicite:17]{index=17}  

- **monitor.c**:  
  - Periodically (every 1 ms) checks if `current_time – last_meal > time_to_die`.  
  - On death: print “died”, set `params->stop = 1`. Monitors exit when `meals_eaten >= meals_required`. :contentReference[oaicite:18]{index=18}  

- **utils.c**:  
  - `current_timestamp_ms()`: ms since start.  
  - `smart_sleep(ms)`: sleeps in small increments, aborting early if `params->stop`.  
  - `print_action()`: locks `print_mutex`, prints `<timestamp> <id> <action>`. :contentReference[oaicite:19]{index=19}  

- **cleanup.c**:  
  - Destroys all mutexes and frees allocated arrays. :contentReference[oaicite:20]{index=20}  

## Build & Run  
```bash
cd philo
make          # builds `philo`
./philo N td te ts [meals_required]
N: number of philosophers/forks.

td: time_to_die (ms).

te: time_to_eat (ms).

ts: time_to_sleep (ms).

[meals_required] (optional): each philosopher must eat this many times. 

Examples
No meal limit (ends on death):

bash
Copy
Edit
./philo 5 800 200 200
