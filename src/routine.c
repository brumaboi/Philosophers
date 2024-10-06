/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 12:00:54 by marvin            #+#    #+#             */
/*   Updated: 2024/09/30 12:00:54 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../inc/philo.h"

static int lock_forks(t_philo *philo, t_data *data)
{
    int first_fork;
    int second_fork;

    if (philo->id % 2 == 0)
    {
        first_fork = philo->left_fork;
        second_fork = philo->right_fork;
    }
    else
    {
        first_fork = philo->right_fork;
        second_fork = philo->left_fork;
    }
    while (1)
    {
        pthread_mutex_lock(&data->forks[first_fork]);
        printf("Philosopher %d locked fork %d\n", philo->id, first_fork);
        if (pthread_mutex_trylock(&data->forks[second_fork]) == 0)
        {
            printf("Philosopher %d locked fork %d\n", philo->id, second_fork);
            return (0);
        }
        else
        {
            pthread_mutex_unlock(&data->forks[first_fork]);
            ft_usleep(50);
        }
        if (one_dead(data))
            return (1);
    }
}

static void unlock_forks(t_philo *philo, t_data *data)
{
    int first_fork;
    int second_fork;

    if (philo->left_fork < philo->right_fork)
    {
        first_fork = philo->left_fork;
        second_fork = philo->right_fork;
    }
    else
    {
        first_fork = philo->right_fork;
        second_fork = philo->left_fork;
    }
    pthread_mutex_unlock(&data->forks[second_fork]);
    pthread_mutex_unlock(&data->forks[first_fork]);
}

static void ft_eat(t_philo *philo, t_data *data)
{
    if (one_dead(data))
        return ;
    if (lock_forks(philo, data) == 1)
        return ;
    print_action(data, philo, "is eating");
    pthread_mutex_lock(&philo->mutex);
    philo->last_meal = ft_get_time();
    pthread_mutex_unlock(&philo->mutex);
    ft_usleep(data->time_to_eat);
    pthread_mutex_lock(&philo->mutex);
    philo->meals++;
    pthread_mutex_unlock(&philo->mutex);
    if (data->meal_count != -1 && philo->meals >= data->meal_count)
    {
        pthread_mutex_lock(&data->full_mutex);
        data->full++;
        pthread_mutex_unlock(&data->full_mutex);
    }
    unlock_forks(philo, data);
    ft_usleep(50);
}

void *routine(void *arg)
{
    t_philo *philo;
    t_data *data;

    philo = (t_philo *)arg;
    data =  philo->data;
    if (philo->id % 2 == 0)
        ft_usleep(data->time_to_eat / 2);
    while(one_dead(data) == 0)
    {
        print_action(data, philo, "is thinking");
        ft_eat(philo, data);
        ft_sleep(philo, data);
    }
    return (NULL);
}
