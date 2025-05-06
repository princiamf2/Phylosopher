#ifndef PHYLOSOPHER_H
#define PHYLOSOPHER_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct s_philo
{
    long    id;
    int     time_for_eat;
    int     time_for_die;
    int     time_for_think;
    int     time_for_sleep;
    int     eat_count;
    int     last_meal_time;
    int     total_eat;
} t_philo;

typedef struct s_data
{
    int     number_philo;
    pthread_mutex_t *mutex;
    pthread_mutex_t print;
    t_philo     *philo;
} t_data;

typedef struct s_philo_arg
{
    int index;
    t_data  *data;
} t_philo_arg;

t_data *allocate_data(int nb_philo);
t_data *clean_data(t_data *data, int err);
int init_print_mutex(t_data *data);
int init_mutex(t_data *data);
void init_philo(t_data *data, int *times);
t_data *init_data(int nb_philo, int *times);
long    get_timestamp(void);
void    ft_sleep(int ms);
void release_forks(t_data *data, int idx);
void    take_forks(t_data *data, int idx);
void    eat(t_data *data, int idx);
void sleep_and_think(t_data *data, int idx);
void *monitor_routine(void *arg);
#endif
