#ifndef PHYLOSOPHER_H
#define PHYLOSOPHER_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct s_args
{
    int     nb_philo;
    long    time_to_die;
    long    time_to_eat;
    long    time_to_sleep;
    int     meals_required;
}   t_args;

typedef struct s_philo
{
    int             id;
    long            last_meal;
    int             eats;
    int             *stop;
    pthread_mutex_t meal_mutex;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
    pthread_mutex_t *print_mutex;
    t_args          *args;
    long            *start_time;
} t_philo;

typedef struct s_data
{
    t_args  args;
    pthread_mutex_t *forks;
    pthread_mutex_t print_mutex;
    pthread_t       *threads;
    pthread_t       monitor;
    t_philo         *philos;
    int             stop;
    long            start_time;
} t_data;

long	get_timestamp(void);
void	ft_sleep(long ms);
void	safe_log(t_philo *ph, const char *msg);
int		init_data(t_data *data, int argc, char **argv);
void	cleanup_data(t_data *data);
void	*philo_routine(void *arg);
void	*monitor_routine(void *arg);
void	eat_action(t_philo *ph);
void	sleep_action(t_philo *ph);
void	think_action(t_philo *ph);

#endif
