/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 12:02:51 by marvin            #+#    #+#             */
/*   Updated: 2024/09/30 12:02:51 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../inc/philo.h"

int ft_atoi(const char *str)
{
    int i;
    int sign;
    long nbr;

    i = 0;
    sign = 1;
    nbr = 0;
    while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n'
        || str[i] == '\v' || str[i] == '\f' || str[i] == '\r')
        i++;
    if (str[i] == '-' || str[i] == '+')
    {
        if (str[i] == '-')
            sign = -1;
        i++;
    }
    while (str[i] >= '0' && str[i] <= '9')
    {
        nbr = nbr * 10 + (str[i] - '0');
        i++;
    }
    return (nbr * sign);
}

void print_action(t_data *data, t_philo *philo, const char *str)
{
    pthread_mutex_lock(&data->print);
    if(one_dead(data) == 1)
    {
        pthread_mutex_unlock(&data->print);
        return ;
    }
    printf("%lld %d %s\n", ft_get_time() - data->start_time, philo->id + 1, str);
    pthread_mutex_unlock(&data->print);
}

int one_dead(t_data *data)
{
    pthread_mutex_lock(&data->dead_mutex);
    if (data->death)
    {
        pthread_mutex_unlock(&data->dead_mutex);
        return (1);
    }
    pthread_mutex_unlock(&data->dead_mutex);
    return (0);
}

void set_death(t_data *data)
{
    pthread_mutex_lock(&data->dead_mutex);
    data->death = 1;
    pthread_mutex_unlock(&data->dead_mutex);
}

int will_starve(t_philo *philo, t_data *data)
{
    long long time_since_last_meal;

    pthread_mutex_lock(&philo->mutex);
    time_since_last_meal = ft_get_time() - philo->last_meal;
    pthread_mutex_unlock(&philo->mutex);
    if (time_since_last_meal > data->time_to_die)
    {
        set_death(data);
        pthread_mutex_lock(&data->print);
        printf("%lld %d died\n", ft_get_time() - data->start_time, philo->id + 1);
        pthread_mutex_unlock(&data->print);
        return (1);
    }
    return (0);
}
