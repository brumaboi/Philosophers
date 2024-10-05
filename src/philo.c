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

long long ft_get_time(void)
{
    struct timeval time;

    gettimeofday(&time, NULL);
    return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void    ft_usleep(useconds_t usec)
{
    long long start;
    long long elapsed;

    start = ft_get_time();
    elapsed = 0;
    while (elapsed < usec)
    {
        usleep(100);
        elapsed = ft_get_time() - start;
    }
}

int one_dead(t_data *data)
{
    pthread_mutex_lock(&data->dead_mutex);
    if (data->death)
    {
        pthread_mutex_unlock(&data->dead_mutex);
        return (0);
    }
    pthread_mutex_unlock(&data->dead_mutex);
    return (1);
}

void ft_sleep(t_philo *philo, t_data *data)
{
    print_action(data, philo, "is sleeping");
    ft_usleep(data->time_to_sleep);
}

int give_forks_not_dead(t_philo *philo, t_data *data)
{
    lock_forks(philo, data);
    pthread_mutex_lock(&philo->mutex);
    will_starve(philo, data);
    if(one_dead(data) == 1)
    {
        pthread_mutex_unlock(&philo->mutex);
        unlock_forks(philo, data);
        return (1);
    }
    pthread_mutex_unlock(&philo->mutex);
    return (0);
}

lock_forks(t_philo *philo, t_data *data)
{
    
}

unlock_forks(t_philo *philo, t_data *data)
{

}

void ft_eat(t_philo *philo, t_data *data)
{
    if (give_forks_not_dead(philo, data) == 1)
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
}

void *routine(void *arg)
{
    t_philo *philo;
    t_data *data;

    philo = (t_philo *)arg;
    data =  philo->data;
    if (philo->id % 2 == 0)
        ft_usleep(data->time_to_eat / 2);
    while(one_dead(data) == 1)
    {
        print_action(data, philo, "is thinking");
        ft_eat(philo, data);
        ft_sleep(philo, data);
    }
    return (NULL);
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

void set_death(t_data *data)
{
    pthread_mutex_lock(&data->dead_mutex);
    data->death = 1;
    pthread_mutex_unlock(&data->dead_mutex);
}

int will_starve(t_philo *philo, t_data *data)
{
    if (ft_get_time() - philo->last_meal > data->time_to_die)
    {
        set_death(data);
        pthread_mutex_lock(&data->print);
        printf("%lld %d died\n", ft_get_time() - data->start_time, philo->id + 1);
        pthread_mutex_unlock(&data->print);
        return (1);
    }
    return (0);
}

void check_philos(t_data *data)
{
    int i;

    while(1)
    {
        i = 0; 
        while (i < data->philo_count)
        {
            if(will_starve(&data->philos[i], data) == 1)
                return ;
            i++;
        }
        pthread_mutex_lock(&data->full_mutex);
        if (data->full == data->philo_count)
        {
            set_death(data);
            pthread_mutex_unlock(&data->full_mutex);
            return ;
        }
        pthread_mutex_unlock(&data->full_mutex);
        ft_usleep(100);
    }
}

void create_philos(t_data *data)
{
    int i;

    i = 0;
    while (i < data->philo_count)
    {
        if (pthread_create(&data->philos[i].thread, NULL, routine, &data->philos[i]))
        {
            set_death(data);
            break ;
        }
        i++;
    }
    check_philos(data);
    i = 0;
    while (i < data->philo_count)
    {
        if (pthread_join(data->philos[i].thread, NULL))
            return (printf("Error: Thread join failed\n"));
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
    create_philos(data);
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
