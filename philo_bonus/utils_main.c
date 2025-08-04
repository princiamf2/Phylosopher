/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 15:32:53 by michel            #+#    #+#             */
/*   Updated: 2025/07/04 14:37:04 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phylosopher_bonus.h"

int	check_argc(int ac, char **av)
{
	(void)av;
	if (ac < 5 || ac > 6)
	{
		ft_putstr_fd("Usage: ./philos_bonus nb_philo time_to_die time_to_eat "
			"time_to_sleep [meals_required]",
			2);
		return (-1);
	}
	return (0);
}

int	parse_args(t_data2 *data, int ac, char **av)
{
	data->args.nb_philo = ft_atoi(av[1]);
	data->args.time_to_die = ft_atoi(av[2]);
	data->args.time_to_eat = ft_atoi(av[3]);
	data->args.time_to_sleep = ft_atoi(av[4]);
	if (ac == 6)
		data->args.meals_required = ft_atoi(av[5]);
	else
		data->args.meals_required = -1;
	if (data->args.nb_philo <= 0 || data->args.time_to_die < 0
		|| data->args.time_to_eat < 0 || data->args.time_to_sleep < 0
		|| (ac == 6 && data->args.meals_required < 0))
	{
		ft_putstr_fd("Error: invalid argument(s)", 2);
		return (-1);
	}
	return (0);
}

int	setup_resources(t_data2 *data, pid_t **pids)
{
	*pids = malloc(sizeof(pid_t) * data->args.nb_philo);
	if (!*pids)
	{
		ft_putstr_fd("malloc", 2);
		return (-1);
	}
	data->pids = *pids;
	if (open_semaphores(data))
		return (-1);
	return (0);
}

void	kill_all(pid_t *pids, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		kill(pids[i], SIGKILL);
		i++;
	}
}

void	wait_all(pid_t *pids, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		waitpid(pids[i], NULL, 0);
		i++;
	}
}
