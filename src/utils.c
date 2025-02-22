/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nal-haki <nal-haki@student.42beirut.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 18:14:03 by nal-haki          #+#    #+#             */
/*   Updated: 2025/01/23 22:29:47 by nal-haki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	print_status(t_philo *philo, char *str, int force_print)
{
	if (pthread_mutex_lock(&(philo->table->write_lock)))
		return (1);
	if (!philo->table->has_finished || force_print)
		printf("%zu\t%d %s\n", timestamp(philo->table), philo->id, str);
	if (pthread_mutex_unlock(&(philo->table->write_lock)))
		return (1);
	return (0);
}

size_t	timestamp(t_table *table)
{
	return (get_current_time() - table->start_time);
}

/* @brief Gets current time in milliseconds since Epoch.
 * @return Time in milliseconds
 */
size_t	get_current_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		write(2, "Time of day Error\n", 18);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

/* @brief A more accurate implementation of usleep().
 * Sleeps for 500 microseconds while the target sleep time has not been met.
 * This balances CPU usage and precision.
 * @param milliseconds Time in milliseconds to sleep for.
 * @return 0 on success
 */
int	ft_usleep(size_t milliseconds)
{
	size_t	start;

	start = get_current_time();
	while ((get_current_time() - start) < milliseconds)
		usleep(500);
	return (0);
}
