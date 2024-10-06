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

void lock_forks(t_philo *philo, t_data *data)
{
    if (philo->left_fork < philo->right_fork)
    {
        pthread_mutex_lock(&data->forks[philo->left_fork]);
        print_action(data, philo, "has taken a fork");
        pthread_mutex_lock(&data->forks[philo->right_fork]);
        print_action(data, philo, "has taken a fork");
    }
    else if (philo->left_fork > philo->right_fork)
    {
        pthread_mutex_lock(&data->forks[philo->right_fork]);
        print_action(data, philo, "has taken a fork");
        pthread_mutex_lock(&data->forks[philo->left_fork]);
        print_action(data, philo, "has taken a fork");
    }
    else
    {
        pthread_mutex_lock(&data->forks[philo->left_fork]);
        print_action(data, philo, "has taken a fork");
        if (one_dead(data))
        {
            pthread_mutex_unlock(&data->forks[philo->left_fork]);
            return ;
        }
        ft_usleep(100);
    }
}

void unlock_forks(t_philo *philo, t_data *data)
{
    if (philo->left_fork < philo->right_fork)
    {
        pthread_mutex_unlock(&data->forks[philo->left_fork]);
        pthread_mutex_unlock(&data->forks[philo->right_fork]);
    }
    else if (philo->left_fork > philo->right_fork)
    {
        pthread_mutex_unlock(&data->forks[philo->right_fork]);
        pthread_mutex_unlock(&data->forks[philo->left_fork]);
    }
    else
    {
        pthread_mutex_unlock(&data->forks[philo->left_fork]);
    }
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
    while(one_dead(data) == 0)
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
    pthread_mutex_lock(&philo->mutex);
    if (ft_get_time() - philo->last_meal > data->time_to_die)
    {
        set_death(data);
        pthread_mutex_lock(&data->print);
        printf("%lld %d died\n", ft_get_time() - data->start_time, philo->id + 1);
        pthread_mutex_unlock(&data->print);
        pthread_mutex_unlock(&philo->mutex);
        return (1);
    }
    pthread_mutex_unlock(&philo->mutex);
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
            clean_mutex(data, i);
            break ;
        }
        i++;
    }
    check_philos(data);
    i = 0;
    while (i < data->philo_count)
    {
        pthread_join(data->philos[i].thread, NULL);
        i++;
    }
    clean_mutex(data, data->philo_count);
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

int clean_mutex(t_data *data, int i)
{
    int j;

    j = 0;
    while (j < i)
    {
        pthread_mutex_destroy(&data->philos[j].mutex);
        pthread_mutex_destroy(&data->forks[j]);
        j++;
    }
    pthread_mutex_destroy(&data->print);
    pthread_mutex_destroy(&data->dead_mutex);
    pthread_mutex_destroy(&data->full_mutex);
    free(data->philos);
    free(data->forks);
    return (1);
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
    while (i < data->philo_count)
    {
        if (pthread_mutex_init(&data->philos[i].mutex, NULL))
            return (clean_mutex(data, i));
        if (pthread_mutex_init(&data->forks[i], NULL) != 0)
            return (clean_mutex(data, i));
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
    t_data data;

    data.philos = NULL;
    data.forks = NULL;
    if (check_parse_input(&data, argc, argv) == 1)
    {
        if (data.philos)
            free(data.philos);
        if(data.forks)
            free(data.forks);
        return (1);
    }
    else 
    {
        clean_mutex(&data, data.philo_count);
        return (0);
    }
}
