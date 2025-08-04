/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phylosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 19:09:19 by mm-furi           #+#    #+#             */
/*   Updated: 2025/08/04 15:30:10 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phylosopher_bonus.h"

int	eat_and_check(t_data2 *data, int id, long *last_meal, int *meals)
{
	t_args2	*args;

	args = &data->args;
	safe_print(data, id + 1, "is eating");
	*last_meal = get_timestamp();
	usleep(args->time_to_eat * 1000);
	(*meals)++;
	if (args->meals_required > 0 && *meals >= args->meals_required)
	{
		*last_meal = -1;
		return (1);
	}
	return (0);
}

void	do_sleep_and_think(t_data2 *data, int id)
{
	t_args2	*args;

	args = &data->args;
	safe_print(data, id + 1, "is sleeping");
	usleep(args->time_to_sleep * 1000);
	safe_print(data, id + 1, "is thinking");
}

static void	philo_init(t_data2 *data, int id, long *last_meal, int *meals)
{
	t_monitor	*moni;
	pthread_t	watcher;

	*last_meal = get_timestamp();
	*meals = 0;
	moni = malloc(sizeof(*moni));
	if (!moni)
	{
		ft_putstr_fd("malloc", 2);
		return ;
	}
	moni->data = data;
	moni->id = id;
	moni->last_meal = last_meal;
	if (pthread_create(&watcher, NULL, death_monitor, moni) != 0)
		ft_putstr_fd("pthread_create", 2);
	pthread_detach(watcher);
	if (data->args.nb_philo > 1 && id % 2 == 0)
		usleep(id * 1000);
}

static void	philo_loop(t_data2 *data, int id, long *last_meal, int *meals)
{
	t_args2	*args;

	args = &data->args;
	if (args->nb_philo == 1)
		single_philo_behavior(data, id);
	while (1)
	{
		take_forks(data, id);
		if (eat_and_check(data, id, last_meal, meals) == 1)
		{
			drop_forks(data, id);
			exit(EXIT_SUCCESS);
		}
		drop_forks(data, id);
		safe_print(data, id + 1, "is sleeping");
		usleep(args->time_to_sleep * 1000);
		safe_print(data, id + 1, "is thinking");
	}
}

void	routine_philo(t_data2 *data, int id)
{
	long	last_meal;
	int		meals;

	philo_init(data, id, &last_meal, &meals);
	philo_loop(data, id, &last_meal, &meals);
}
