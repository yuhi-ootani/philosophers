# philosophers

## Summary  
This repository implements the **Dining Philosophers** problem in C, following the 42 School project specification. It simulates a set of philosophers sitting at a round table, each requiring two forks (shared resources) to eat. Depending on the build target, it can use either processes with semaphores (mandatory part) or threads with mutexes (bonus part) to coordinate access to forks and to detect philosopher “death” when the time to die elapses without eating. :contentReference[oaicite:0]{index=0}

## Repository Structure  
- **`Makefile`**  
  - Builds two executables:  
    - `philo`: the mandatory version using separate processes and POSIX semaphores.  
    - `philo_bonus`: the bonus version using threads and pthread mutexes.  
  - Targets:  
    - `all` (default): Compiles both `philo` and `philo_bonus`.  
    - `clean`: Removes object files.  
    - `fclean`: Removes object files and the executables (`philo`, `philo_bonus`).  
    - `re`: Runs `fclean` then `all`. :contentReference[oaicite:1]{index=1}

- **`philo.h`**  
  - Central header defining:  
    - Structs for philosopher state, timing parameters, and shared data (e.g., number of philosophers, time_to_die, time_to_eat, time_to_sleep, number_of_times_each_must_eat).  
    - Function prototypes for initialization, monitoring, actions (take forks, eat, sleep, think), cleaning up resources, and utility routines (timestamping, printing).  
    - Includes for standard headers (`<stdio.h>`, `<stdlib.h>`, `<unistd.h>`, `<pthread.h>`, `<semaphore.h>`, `<sys/time.h>`). :contentReference[oaicite:2]{index=2}

- **Source files for mandatory part (`philo/` or top‐level files)**  
  1. **`main.c`**  
     - Parses and validates command-line arguments:  
       ```bash
       ./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
       ```  
     - Initializes shared semaphores (forks semaphore, printing semaphore, and a semaphore to indicate that all philosophers have eaten enough).  
     - Forks one process per philosopher, passing each its index and pointers to shared data.  
     - In each child process, calls `philosopher_routine()` and then exits.  
     - The parent waits for either a “death” signal or for all children to finish eating the required number of times, then cleans up semaphores and processes. :contentReference[oaicite:3]{index=3}

  2. **`philosopher.c`**  
     - Implements `philosopher_routine(void *arg)`:  
       - Records the start timestamp.  
       - Enters a loop:  
         1. Waits (sem_wait) on a “forks” semaphore twice to take two forks.  
         2. Records timestamp and prints “has taken a fork” twice.  
         3. Records timestamp and prints “is eating,” then sleeps for `time_to_eat` milliseconds.  
         4. Posts (sem_post) on the forks semaphore twice to release forks.  
         5. Increments local eaten count; if it reaches `number_of_times_each_must_eat`, signals the “full” semaphore and exits loop.  
         6. Records timestamp and prints “is sleeping,” then sleeps for `time_to_sleep` milliseconds.  
         7. Prints “is thinking.”  
       - Spawns a monitor thread (or uses a separate process in some implementations) that periodically checks `get_timestamp() - last_meal_time > time_to_die`; if true, prints “died” and signals parent to terminate all processes. :contentReference[oaicite:4]{index=4}

  3. **`monitor.c`**  
     - Contains functions to monitor each philosopher’s time-to-die:  
       - In the mandatory version, each child process creates a thread that checks its own death condition.  
       - Upon detecting death, the monitor prints “timestamp X died” and calls `exit(EXIT_FAILURE)`, causing the parent to clean up.  
       - Optionally, a parent process waits on a “full” semaphore N times (for each philosopher) to know when all have eaten enough and can terminate cleanly. :contentReference[oaicite:5]{index=5}

  4. **`utils.c`**  
     - Helper functions for:  
       - `get_timestamp()`: Returns the number of milliseconds since the program started.  
       - `ft_sleep(ms)`: Sleeps for the specified number of milliseconds (using `usleep` in a loop to check for death signals).  
       - `print_action(philo, action_str)`: Uses a printing semaphore to serialize console output in the format:  
         ```
         <timestamp> <philosopher_number> <action_str>
         ```  
       - Error handling and freeing allocated resources: semaphores, allocated arrays, joined threads, and child processes. :contentReference[oaicite:6]{index=6}

- **Bonus part source files (`philo_bonus/` or similarly named)**  
  1. **`main_bonus.c`**  
     - Similar argument parsing and validation as in the mandatory version.  
     - Initializes pthread mutexes for forks (an array of `pthread_mutex_t` of size `number_of_philosophers`).  
     - Creates one thread per philosopher plus a monitor thread for each.  
     - Joins all threads after either detecting death or all philosophers having eaten required times.  
     - Destroys all mutexes and frees memory before exiting. :contentReference[oaicite:7]{index=7}

  2. **`philosopher_bonus.c`**  
     - Implements `philosopher_routine(void *arg)` for the threaded version:  
       - Locks left and right fork mutexes (in a deadlock‐avoidance order: e.g., lower‐indexed first).  
       - Prints “has taken a fork” twice, then “is eating” and sleeps.  
       - Unlocks both fork mutexes, increments eaten count, signals parent via a shared counter or semaphore if the eating target is reached.  
       - Prints “is sleeping” and sleeps for `time_to_sleep` ms, then prints “is thinking.”  
       - Continues until death or eating target reached. :contentReference[oaicite:8]{index=8}

  3. **`monitor_bonus.c`**  
     - In a separate thread per philosopher, checks for death in the same way as mandatory:  
       - Periodically compares `current_time – last_meal_time > time_to_die`.  
       - If a philosopher dies, prints “died” (protected by a printing mutex or semaphore) and signals all other threads to exit. :contentReference[oaicite:9]{index=9}

- **`.gitignore`**  
  - Ignores object files (`*.o`), executables (`philo`, `philo_bonus`), and any editor swapfiles. :contentReference[oaicite:10]{index=10}

## Languages and Tools  
- **C (95.5%)**: All source files implement the simulation logic, timing, synchronization (processes with semaphores or threads with mutexes), and monitoring. :contentReference[oaicite:11]{index=11}  
- **Makefile (4.5%)**: Automates building, cleaning, and rebuilding both `philo` and `philo_bonus` executables. :contentReference[oaicite:12]{index=12}

## How to Build and Run  

1. **Clone the repository**  
   ```bash
   git clone https://github.com/yuhi-ootani/philosophers.git
   cd philosophers
   ``` :contentReference[oaicite:13]{index=13}

2. **Compile**  
   ```bash
   make
