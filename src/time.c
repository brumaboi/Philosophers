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

# include "../inc/philo.h"

long long ft_get_time(void)
{
    struct timeval time;

    gettimeofday(&time, NULL);
    return ((time.tv_sec * 1000000) + time.tv_usec);
}

void    ft_usleep(useconds_t usec)
{
    long long start;
    long long end;
    useconds_t time_left;

    start = ft_get_time();
    end = start + usec;
    while (ft_get_time() < end)
    {
        time_left = end - ft_get_time();
        if (time_left > 100)
            usleep(100);
        else
            usleep(time_left);
    }
}

void ft_sleep(t_philo *philo, t_data *data)
{
    print_action(data, philo, "is sleeping");
    ft_usleep(data->time_to_sleep);
}