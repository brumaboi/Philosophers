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

struct s_data
{
    int philo_count;
    int time_to_die;
    int time_to_eat;
    int time_to_sleep;
    int meal_count;
} t_data;

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

    if (argc > 5 && argc < 6)
        return (printf("Error: Wrong number of arguments\n"), 1);
    i = 0;
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

int parse_input(int argc, char **argv)
{
    t_data.philo_count = ft_atoi(argv[1]);
    t_data.time_to_die = ft_atoi(argv[2]);
    t_data.time_to_eat = ft_atoi(argv[3]);
    t_data.time_to_sleep = ft_atoi(argv[4]);
    if (argc == 6)
        t_data.meal_count = ft_atoi(argv[5]);
    else
        t_data.meal_count = -1;
    if (t_data.philo_count < 1 || t_data.time_to_die < 1 || t_data.time_to_eat < 1
        || t_data.time_to_sleep < 1 || (argc == 6 && t_data.meal_count < 1))
        return (printf("Error: Invalid argument\n"), 1);
    return (0);
}

int check_parse_input(int argc, char **argv)
{
    if (check_input(argc, argv) == 1)
        return (1);
    if (parse_input(argc, argv) == 1)
        return (1);
    return (0);
}

int main(int argc, char **argv)
{
    if (check_parse_input(argc, argv) == 1)
        return (1);
}