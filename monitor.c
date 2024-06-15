#include "philo.h"

// Funkcja wyspisuje wiadomosc w bezpieczny sposob w kontekscie wielowatkowym

void    print_message(char *str, t_philo *philo, int id) // Przyjmuje wiadomosc do wypisania (*str), wskaznik do struktury (*philo) oraz ID
{
	size_t	time;

	pthread_mutex_lock(philo->write_lock); // blokuje mutex "write_lock", ktory zapewnia ze tylko jeden watek na raz bedzie mogl miec dostep do sekcji krytycznej, w tym przypadku do wywolan funkcji "printf"
	time = get_current_time() - philo->start_time; // oblicza roznice miedzy aktualnym czasem, a czasem rozpoczecia dzialani programu (przechowywanym w philo->start_time)
	if(!dead_loop(philo)) // jesli jezeli zmienna "dead" w strukturze philo jest falszywa (czyli 0, znaczace ze watek powinien kontynuowac dzialanie) to wypisujemy wiadomosc ...
		printf("%zu %d %s\n", time, id, str); // ... w ktorej zawieram informacje o czasie pracy programu, ID oraz wypisuje wiadomosc
	pthread_mutex_unlock(philo->write_lock); // odblokowuje mutex. Odblokowanie jest niezbedne aby inne watki mogly uzyskac dostep do sekcji krytycznej, gdy funkcja "print_message" zostanie zakonczona
}

// Sprawdza czy dany filozof jest martwy (czyli czy zakonczyl wykonywanie zadania)
// Zwraca 1 jesli filozof jest martwy (prawda)
// Zwraca 0 jesli filozof zyje (falsz)

int	philo_dead(t_philo *philo, size_t time_to_die)
{
	pthread_mutex_lock(philo->meal_lock); // blokuje mutex "meal_lock", ktory zapewnia ze tylko jeden watek na raz bedzie mogl miec dostep do sekcji krytycznej, czyli do "last_meal" i "eating" w strukturze "philo"
	if((get_current_time() - philo->last_meal >= time_to_die) && philo->eating == 0) // jezeli czas od ostatniego posilku (get_current_time() - philo->last_meal) jest dluzszy od maksymalnego czasu jaki moze przezyc filozof bez posilku (time_to_die)
	{
		pthread_mutex_unlock(philo->meal_lock);
		return(1);
	}
	pthread_mutex_unlock(philo->meal_lock);
	return(0);
}