/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phylosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 19:09:19 by mm-furi           #+#    #+#             */
/*   Updated: 2025/06/26 18:15:00 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phylosopher_bonus.h"

int	eat_and_check(t_data2 *data, int id, long *last_meal, int *meals)
{
	t_args2	*args;

	args = &data->args;
	safe_print(data, id, "is eating");
	usleep(args->time_to_eat * 1000);
	*last_meal = get_timestamp();
	(*meals)++;
	if (args->meals_required > 0 && *meals >= args->meals_required)
	{
		drop_forks(data);
		exit(EXIT_SUCCESS);
	}
	return (1);
}

void	do_sleep_and_think(t_data2 *data, int id)
{
	t_args2	*args;

	args = &data->args;
	safe_print(data, id, "is sleeping");
	usleep(args->time_to_sleep * 1000);
	safe_print(data, id, "is thinking");
}

void	routine_philo(t_data2 *data, int id)
{
	long		last_meal;
	int			meals;
	t_args2		*args;
	pthread_t	watcher;
	t_monitor	*moni;

	args = &data->args;
	last_meal = get_timestamp();
	meals = 0;
	moni = malloc(sizeof(*moni));
	moni->data = data;
	moni->id = id;
	moni->last_meal = &last_meal;
	if (pthread_create(&watcher, NULL, death_monitor, moni) != 0)
		perror("pthread_create");
	pthread_detach(watcher);
	if (id % 2 == 1)
		usleep(data->args.time_to_eat * 1000);
	while (1)
	{
		take_forks(data);
		safe_print(data, id, "has taken a fork");
		safe_print(data, id, "has taken a fork");
		eat_and_check(data, id, &last_meal, &meals);
		drop_forks(data);
		safe_print(data, id, "is sleeping");
		usleep(args->time_to_sleep * 1000);
		safe_print(data, id, "is thinking");
	}
}
