/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:41:07 by michel            #+#    #+#             */
/*   Updated: 2025/06/24 15:39:19 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phylosopher.h"

void	one_philo(t_philo *ph)
{
	safe_log(ph, "has taken a fork");
	ft_sleep(ph->args->time_to_die);
	safe_log(ph, "died");
	ph->data->stop = 1;
}

void	stoppable_sleep(t_philo *ph, long ms)
{
	long	start;

	start = get_timestamp();
	while (!ph->data->stop && get_timestamp() - start < ms)
		usleep(1000);
}

int	main(int argc, char **argv)
{
	t_data	data;
	int		i;

	if (init_data(&data, argc, argv))
		return (1);
	i = 0;
	while (i < data.args.nb_philo)
	{
		pthread_create(&data.threads[i], NULL, philo_routine, &data.philos[i]);
		i++;
	}
	pthread_create(&data.monitor, NULL, monitor_routine, &data);
	if (data.stop == 1)
		return (0);
	i = 0;
	while (i < data.args.nb_philo)
	{
		pthread_join(data.threads[i], NULL);
		i++;
	}
	pthread_join(data.monitor, NULL);
	cleanup_data(&data);
	return (0);
}
