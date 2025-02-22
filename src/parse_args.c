/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nal-haki <nal-haki@student.42beirut.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 20:44:47 by nal-haki          #+#    #+#             */
/*   Updated: 2025/01/23 22:29:47 by nal-haki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	ft_isspace(char c)
{
	return (('\t' <= c && c <= '\r') || c == ' ');
}

static int	ft_isdigit(int c)
{
	return ('0' <= c && c <= '9');
}

static int	is_int(long nbr)
{
	return (INT_MIN <= nbr && nbr <= INT_MAX);
}

static size_t	ft_atoi_ph(const char *nptr, int *error)
{
	size_t	output;
	size_t	prev_output;

	output = 0;
	*error = 0;
	while (ft_isspace(*nptr))
		nptr++;
	if (!ft_isdigit(*nptr))
		*error = 1;
	while (ft_isdigit(*nptr))
	{
		prev_output = output;
		output = (output * 10) + (*nptr++ - '0');
		if (output < prev_output)
			*error = 1;
	}
	if (*nptr != '\0')
		*error = 1;
	return (output);
}

int	parse_args(int ac, char **av, t_table *table)
{
	int	error;

	if (ac < 5 || ac > 6)
		return (printf("Invalid number of args\n"), 1);
	table->philo_number = ft_atoi_ph(av[1], &error);
	if (error || table->philo_number < 1 || table->philo_number > MAX_PHILOS)
		return (printf("Invalid number_of_philosophers\n"), 2);
	table->die_time = ft_atoi_ph(av[2], &error);
	if (error || table->die_time < 60)
		return (printf("Invalid time to die\n"), 3);
	table->eat_time = ft_atoi_ph(av[3], &error);
	if (error || table->eat_time < 60)
		return (printf("Invalid time to eat\n"), 4);
	table->sleep_time = ft_atoi_ph(av[4], &error);
	if (error || table->sleep_time < 60)
		return (printf("Invalid time to sleep\n"), 5);
	if (ac == 6)
	{
		table->is_meal_required = 1;
		table->required_meal_number = ft_atoi_ph(av[5], &error);
		if (error || !is_int(table->required_meal_number)
			|| table->required_meal_number <= 0)
			return (printf("Invalid times philosophers must eat\n"), 6);
	}
	return (0);
}
