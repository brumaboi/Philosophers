/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 12:14:13 by marvin            #+#    #+#             */
/*   Updated: 2024/09/30 12:14:13 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

long long	ft_get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	ft_usleep(useconds_t usec, t_data *data)
{
	long long	start;
	long long	end;
	long long	now;

	start = ft_get_time();
	end = start + usec;
	while (!one_dead(data))
	{
		now = ft_get_time();
		if (now >= end)
			break ;
		usleep(200);
	}
}

void	ft_sleep(t_philo *philo, t_data *data)
{
	print_action(data, philo, "is sleeping");
	ft_usleep(data->time_to_sleep, data);
}
