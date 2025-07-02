/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phylosopher_bonus.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: michel <michel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 18:58:39 by mm-furi           #+#    #+#             */
/*   Updated: 2025/07/02 15:42:46 by michel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHYLOSOPHER_BONUS_H
#define PHYLOSOPHER_BONUS_H

#include "../phylosopher.h"
#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>

typedef struct s_args2 {
  int nb_philo;
  long time_to_die;
  long time_to_eat;
  long time_to_sleep;
  int meals_required;
} t_args2;

typedef struct s_data2 {
  t_args2 args;
  long start_time;
  sem_t *forks;
  sem_t *print;
  sem_t *death_sem;
  pid_t *pids;
} t_data2;

typedef struct s_monitor {
  t_data2 *data;
  int id;
  long *last_meal;
} t_monitor;

sem_t *open_semaphores(t_data2 *data, int n);
void close_semaphores(t_data2 *data);
long get_timestamp(void);
void safe_print(t_data2 *data, int id, const char *msg);
void drop_forks(t_data2 *data);
void take_forks(t_data2 *data);
void routine_philo(t_data2 *data, int id);
void *death_monitor(void *arg);
int check_argc(int ac, char **av);
int  parse_args(t_data2 *data, int ac, char **av);
int  setup_resources(t_data2 *data, pid_t **pids);
void	kill_all(pid_t *pids, int n);
void	wait_all(pid_t *pids, int n);
#endif
