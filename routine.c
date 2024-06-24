/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrechuli <mrechuli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 18:40:47 by mrechuli          #+#    #+#             */
/*   Updated: 2024/06/24 18:40:49 by mrechuli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void    think(t_philo *philo)
{
    print_message("Is thinking", philo, philo->id);
}

void	dream(t_philo *philo) // funkcja nazywa sie "dream", a nie sleep, poniewaz w C jest funkcja ktora nazywa sie sleep
{
	print_message("Is sleeping", philo, philo->id);
	ft_usleep(philo->time_to_sleep);
}

void	eat(t_philo *philo)
{
	pthread_mutex_lock(philo->r_fork); // filozof probuje podniesc prawy widelec blokujac odpowiedni mutex
	print_message("Has taken a right fork", philo, philo->id); // drukuje komunikat ze wzial widelec
	if(philo->num_of_philos == 1) // jesli filozof jest tylko jeden (czyli jest tylko prawy widelec) ...
	{
		ft_usleep(philo->time_to_die); // to usypiam dzialanie programu na czas rowny "time_to_die" (czyli czasu ktory jest potrzebny aby filozof umarl; jest to potrzebne do prawidlowego zakonczenia dzialaniu programu, bo 1 filozof nie jest w stanie podniesc lewego widelca bo go nie ma)
		pthread_mutex_unlock(philo->r_fork); // odblokowuje mutex, ktory wczesniej zablokowalem
		return ; // koncze dzialanie funkcji
	}
	pthread_mutex_lock(philo->l_fork); // filozof probuje podniesc lewy widelec blokujac odpowiedni mutex; jesli nie moze tego zrobic to czeka w kolejce
	print_message("Has taken a left fork", philo, philo->id); // drukuje komunikat ze wzial widelec
	philo->eating = 1; // zmienna "eating" jest ustawiana na 1, czyli, ze filozof je
	print_message("Is eating", philo, philo->id); // drukuje komunikat ze filozof je
	pthread_mutex_lock(philo->meal_lock); // blokuje mutex "meal_lock"
	philo->last_meal = get_current_time(); // aktualizuje czas ostatniego posilku "last meal" na aktualny czas 
	philo->meals_eaten++; // inkrementuje ilosc zjedzonych posilkow
	pthread_mutex_unlock(philo->meal_lock); // odblokowanie mutexu "meal_lock"
	ft_usleep(philo->time_to_eat); // usypia watek na czas jedzenia "time_to_eat"
	philo->eating = 0; // zmieniam "eating" filozofa na 0 (czyli filozof skonczyl jesc)
	pthread_mutex_unlock(philo->l_fork); // odblokowuje mutex "l_fork" i "r_fork", co oznacza ze filozof odlozyl widelce
	pthread_mutex_unlock(philo->r_fork);
}