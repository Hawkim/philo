/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nal-haki <nal-haki@student.42beirut.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 12:19:43 by nal-haki          #+#    #+#             */
/*   Updated: 2025/01/23 08:59:28 by nal-haki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	initialise_philos(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->philo_number)
	{
		data->philos[i].id = i + 1;
		data->philos[i].left_fork = i;
		data->philos[i].right_fork = (i + 1) % data->philo_number;
		data->philos[i].data = data;
	}
	return (0);
}

int	initialize_mutexes(pthread_mutex_t *mutexes, int count)
{
	int	i;
	int	ret;

	i = -1;
	while (++i < count)
	{
		ret = pthread_mutex_init(&mutexes[i], NULL);
		if (ret != 0)
		{
			while (--i >= 0)
				pthread_mutex_destroy(&mutexes[i]);
			return (ret);
		}
	}
	return (0);
}

int	init_data(int ac, char **av, t_data *data)
{
	int	err;

	memset(data, 0, sizeof(*data));
	if (parse_args(ac, av, data) != 0)
		return (1);
	if (data->is_meal_required && data->required_meal_number == 0)
		return (6);
	err = pthread_mutex_init(&data->write_lock, NULL);
	if (err != 0)
		return (write(2, "Error initialising mutex\n", 25), 2);
	err = pthread_mutex_init(&data->last_meal_lock, NULL);
	if (err != 0)
		return (write(2, "Error initialising mutex\n", 25), 2);
	err = pthread_mutex_init(&data->eaten_enough_lock, NULL);
	if (err != 0)
		return (write(2, "Error initialising mutex\n", 25), 2);
	err = initialize_mutexes(data->forks_lock, data->philo_number);
	if (err != 0)
		return (write(2, "Error initialising mutex\n", 25), 3);
	if (initialise_philos(data) != 0)
		return (4);
	return (0);
}