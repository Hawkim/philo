/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nal-haki <nal-haki@student.42beirut.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 12:19:43 by nal-haki          #+#    #+#             */
/*   Updated: 2025/01/23 22:29:47 by nal-haki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	initialise_philos(t_table *table)
{
	int	i;

	i = -1;
	while (++i < table->philo_number)
	{
		table->philos[i].id = i + 1;
		table->philos[i].leftfork = i;
		table->philos[i].rightfork = (i + 1) % table->philo_number;
		table->philos[i].table = table;
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

int	init_table(int ac, char **av, t_table *table)
{
	int	err;

	memset(table, 0, sizeof(*table));
	if (parse_args(ac, av, table) != 0)
		return (1);
	if (table->is_meal_required && table->required_meal_number == 0)
		return (6);
	err = pthread_mutex_init(&table->write_lock, NULL);
	if (err != 0)
		return (write(2, "Mutex Error\n", 13), 2);
	err = pthread_mutex_init(&table->last_meal_lock, NULL);
	if (err != 0)
		return (write(2, "Mutex Error\n", 13), 2);
	err = pthread_mutex_init(&table->eaten_enough_lock, NULL);
	if (err != 0)
		return (write(2, "Mutex Error\n", 13), 2);
	err = initialize_mutexes(table->forks_lock, table->philo_number);
	if (err != 0)
		return (write(2, "Mutex Error\n", 13), 3);
	if (initialise_philos(table) != 0)
		return (4);
	return (0);
}
