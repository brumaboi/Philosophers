/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 12:00:11 by marvin            #+#    #+#             */
/*   Updated: 2024/09/30 12:00:11 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../inc/philo.h"

void *routine(void *arg)
{
    t_philo *philo;
    t_data *data;

    philo = (t_philo *)arg;
    data =  philo->data;
    //// add routine
}

void create_threads(t_data *data)
{
    int i;

    i = 0;
    while (i < data->philo_count)
    {
        if (pthread_create(&data->philos[i].thread, NULL, routine, &data->philos[i]))
            return (printf("Error: Thread creation failed\n")); //add free function
        i++;
    }
    i = 0;
    while (i < data->philo_count)
    {
        if (pthread_join(data->philos[i].thread, NULL))
            return (printf("Error: Thread join failed\n")); //add free function
        i++;
    }
}

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

int check_input(int argc, char **argv)
{
    int i;
    int j;

    if (argc < 5 || argc > 6)
        return (printf("Error: Wrong number of arguments\n"), 1);
    i = 1;
    while(i < argc)
    {
        j = 0;
        while (argv[i][j] != '\0')
        {
            if (argv[i][j] < '0' || argv[i][j] > '9')
                return (printf("Error: Invalid argument\n"), 1);
            j++;
        }
        if (ft_atoi(argv[i]) <= 0)
            return (printf("Error: Invalid argument\n"), 1);
        i++;
    }
    return (0);
}

int parse_input(t_data *data, int argc, char **argv)
{
    data->philo_count = ft_atoi(argv[1]);
    data->time_to_die = ft_atoi(argv[2]);
    data->time_to_eat = ft_atoi(argv[3]);
    data->time_to_sleep = ft_atoi(argv[4]);
    if (argc == 6)
        data->meal_count = ft_atoi(argv[5]);
    else
        data->meal_count = -1;
    if (data->philo_count < 1 || data->time_to_die < 1 || data->time_to_eat < 1
        || data->time_to_sleep < 1 || (argc == 6 && data->meal_count < 1))
        return (printf("Error: Invalid argument\n"), 1);
    data->philos = malloc(sizeof(t_philo) * data->philo_count);
    if (!data->philos)
        return (printf("Error: Malloc failed\n"), 1);
    data->forks = malloc(sizeof(pthread_mutex_t) * data->philo_count);
    if (!data->forks)
        return (printf("Error: Malloc failed\n"), 1);
    if (init_mutex(data) == 1)
        return (printf("Error: Mutex init failed\n"), 1);
    data->start_time = ft_get_time();
    if (init_philos(data) == 1)
        return (printf("Error: Philo init failed\n"), 1);
    return (0);
}

int init_mutex(t_data *data)
{
    int i;

    i = 0;
    if (pthread_mutex_init(&data->print, NULL) != 0)
        return (1);
    if (pthread_mutex_init(&data->dead_mutex, NULL) != 0)
        return (1);
    if (pthread_mutex_init(&data->full_mutex, NULL) != 0)
        return (1);
    if (pthread_mutex_init(&data->ready_mutex, NULL) != 0)
        return (1);
    while (i < data->philo_count)
    {
        if (pthread_mutex_init(&data->philos[i].mutex, NULL))
            return (1);
        if (pthread_mutex_init(&data->forks[i], NULL) != 0)
            return (1);
        i++;
    }
    return (0);
}

int init_philos(t_data *data)
{
    int i;

    i = 0;
    while (i < data->philo_count)
    {
        data->philos[i].id = i;
        data->philos[i].left_fork = i;
        data->philos[i].right_fork = (i + 1) % data->philo_count;
        data->philos[i].meals = 0;
        data->philos[i].last_meal = data->start_time;
        i++;
    }
    create_threads(data);
    return (0);
}

int check_parse_input(t_data *data, int argc, char **argv)
{
    if (check_input(argc, argv) == 1)
        return (1);
    if (parse_input(data, argc, argv) == 1)
        return (1);
    return (0);
}

int main(int argc, char **argv)
{
    t_data *data;

    data = malloc(sizeof(t_data));
    if (!data)
        return (printf("Error: Malloc failed\n"), 1);
    if (check_parse_input(&data, argc, argv) == 1)
        return (1);
    ////
    free(data->philos);
    free(data->forks);
    free(data);
    return(0);
}
