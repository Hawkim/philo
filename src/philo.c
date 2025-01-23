/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nal-haki <nal-haki@student.42beirut.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 14:09:58 by nal-haki          #+#    #+#             */
/*   Updated: 2025/01/23 09:05:39 by nal-haki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
 * av[1] = number_of_philosophers (min 1 / max 200)
 * av[2] = die_time (min 60ms)
 * av[3] = eat_time (min 60ms)
 * av[4] = sleep_time (min 60ms)
 * av[5] = number_of_times_each_philosopher_must_eat [optional]
 */

int	eat(t_philo *philo)
{
	if (philo->id % 2 == 1)
		pthread_mutex_lock(&(philo->data->forks_lock[philo->left_fork]));
	else
		pthread_mutex_lock(&(philo->data->forks_lock[philo->right_fork]));
	print_status(philo, "has taken a fork 🍴", 0);
	if (philo->data->philo_number == 1)
		return (pthread_mutex_unlock(
				&(philo->data->forks_lock[philo->left_fork])), 1);
	if (philo->id % 2 == 1)
		pthread_mutex_lock(&(philo->data->forks_lock[philo->right_fork]));
	else
		pthread_mutex_lock(&(philo->data->forks_lock[philo->left_fork]));
	print_status(philo, "has taken a fork 🍴", 0);
	pthread_mutex_lock(&philo->data->last_meal_lock);
	philo->last_meal_time = timestamp(philo->data);
	pthread_mutex_unlock(&philo->data->last_meal_lock);
	print_status(philo, "is eating 🍝", 0);
	ft_usleep(philo->data->eat_time);
	meals_count_util(philo);
	pthread_mutex_unlock(&(philo->data->forks_lock[philo->right_fork]));
	pthread_mutex_unlock(&(philo->data->forks_lock[philo->left_fork]));
	print_status(philo, "is sleeping 💤", 0);
	ft_usleep(philo->data->sleep_time);
	print_status(philo, "is thinking 💡", 0);
	return (0);
}

void	*monitor(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (1)
	{
		if (is_dead(data))
			return (NULL);
		if (data->is_meal_required && meals_check(data))
		{
			pthread_mutex_lock(&data->write_lock);
			data->has_finished = 1;
			return (pthread_mutex_unlock(&data->write_lock), NULL);
		}
		usleep(100);
	}
	return (NULL);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		ft_usleep(philo->data->eat_time);
	while (1)
	{
		if (eat(philo))
		{
			pthread_mutex_lock(&philo->data->write_lock);
			philo->data->has_finished = 1;
			return (pthread_mutex_unlock(&philo->data->write_lock), NULL);
		}
		pthread_mutex_lock(&philo->data->write_lock);
		if (philo->data->has_finished == 1)
			return (pthread_mutex_unlock(&philo->data->write_lock), NULL);
		pthread_mutex_unlock(&philo->data->write_lock);
		if (philo->data->philo_number % 2 == 1)
			ft_usleep((philo->data->eat_time * 2)
				- (philo->data->sleep_time));
	}
	return (NULL);
}

void	run_philos(t_data *data)
{
	int	i;

	pthread_create(&data->monitor, NULL, monitor, (void *)data);
	i = -1;
	while (++i < data->philo_number)
	{
		pthread_mutex_lock(&data->last_meal_lock);
		data->philos[i].last_meal_time = timestamp(data);
		pthread_create(&data->philos[i].thread, NULL, routine,
			(void *)&data->philos[i]);
		pthread_mutex_unlock(&data->last_meal_lock);
	}
}

int	main(int ac, char **av)
{
	t_data	data;

	if (init_data(ac, av, &data) != 0)
		return (1);
	data.start_time = get_current_time();
	run_philos(&data);
	threads_join(&data);
	destroy_locks(&data);
	return (0);
}
