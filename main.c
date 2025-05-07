/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: michel <michel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:41:07 by michel            #+#    #+#             */
/*   Updated: 2025/05/07 18:08:01 by michel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phylosopher.h"

int	main(int argc, char **argv)
{
	t_data	data;
	int		i;

	if (init_data(&data, argc, argv))
		return (1);
    i = 0;
    while (i < data.args.nb_philo)
    {
        pthread_create(&data.threads[i], NULL,
                       philo_routine, &data.philos[i]);
        i++;
    }
    
    pthread_create(&data.monitor, NULL,
                   monitor_routine, &data);
    
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
