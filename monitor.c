#include "philo.h"

// Funkcja wyspisuje wiadomosc w bezpieczny sposob w kontekscie wielowatkowym

void    print_message(char *str, t_philo *philo, int id) // Przyjmuje wiadomosc do wypisania (*str), wskaznik do struktury (*philo) oraz ID
{
	size_t	time;

	pthread_mutex_lock(philo->write_lock); // blokuje mutex "write_lock", ktory zapewnia ze tylko jeden watek na raz bedzie mogl miec dostep do sekcji krytycznej, w tym przypadku do wywolan funkcji "printf"
	time = get_current_time() - philo->start_time; // oblicza roznice miedzy aktualnym czasem, a czasem rozpoczecia dzialani programu (przechowywanym w philo->start_time)
	if(!dead_loop(philo)) // jezeli zmienna "dead" w strukturze philo jest falszywa (czyli 0, znaczace ze watek powinien kontynuowac dzialanie) to wypisujemy wiadomosc ...
		printf("%zu %d %s\n", time, id, str); // ... w ktorej zawieram informacje o czasie pracy programu, ID oraz wypisuje wiadomosc
	pthread_mutex_unlock(philo->write_lock); // odblokowuje mutex. Odblokowanie jest niezbedne aby inne watki mogly uzyskac dostep do sekcji krytycznej, gdy funkcja "print_message" zostanie zakonczona
}

// Sprawdza czy dany filozof jest martwy (czyli czy zakonczyl wykonywanie zadania)
// Zwraca 1 jesli filozof jest martwy (prawda)
// Zwraca 0 jesli filozof zyje (falsz)

int	philo_dead(t_philo *philo, size_t time_to_die)
{
	pthread_mutex_lock(philo->meal_lock); // blokuje mutex "meal_lock", ktory zapewnia ze tylko jeden watek na raz bedzie mogl miec dostep do sekcji krytycznej, czyli do "last_meal" i "eating" w strukturze "philo"
	if((get_current_time() - philo->last_meal >= time_to_die) && philo->eating == 0) // jezeli czas od ostatniego posilku (get_current_time() - philo->last_meal) jest dluzszy od maksymalnego czasu jaki moze przezyc filozof bez posilku (time_to_die) i jesli filozof akurat nie je
	{
		pthread_mutex_unlock(philo->meal_lock);
		return(1);
	}
	pthread_mutex_unlock(philo->meal_lock);
	return(0);
}

// Sprawdza czy ktorykolwiek z filozofow umarl z glodu.
// Jesli znajdzie martwego filozofa to drukuje odpowiedni komunikat,
// ustawia flage "dead" na 1 i zwraca 1
// Jesli zaden filozof nie umarl to funkcja zwraca 0.

int	check_if_any_dead(t_philo *philos)
{
	int	x;

	x = 0;
	while(x < philos[0].num_of_philos) // petla iterujaca przez wszystkich filozofow
	{
		if(philo_dead(&philos[x], philos[x].time_to_die)) // sprawdzam czy umarl, jesli tak to wchodzimy w "ifa"
		{
			print_message("died", &philos[x], philos[x].id);
			pthread_mutex_lock(philos[0].dead_lock); // blokujemy mutex ...
			*philos->dead = 1; // ... ustawiamy flage na 1 ...
			pthread_mutex_unlock(philos[0].dead_lock); // ... odblokowujemy mutex
			return(1);
		}
		x++;
	}
	return(0);
}

// Funkcja sprawdza czy wszyscy filozofowie zjedli wymagana ilosc posilkow.
// Jesli tak to ustawia flage "dead" na 1 i zwraca 1.
// Jesli nie to po prostu zwraca 0.

int	check_if_all_ate(t_philo *philos)
{
	int	x;
	int	finished_eating; // argument liczacy filozofow ktorzy zjedli wymagana ilosc posilkow

	x = 0;
	finished_eating = 0;
	if(philos[0].num_times_to_eat == -1) // oznacza ze nie ma ograniczenia co do liczby posilkow, wiec zwraca 0.
		return(0);
	while(x < philos[0].num_of_philos) // petla przechodzaca przez wszystkich filozofow
	{
		pthread_mutex_lock(philos[x].meal_lock);
		if(philos[x].meals_eaten >= philos[x].num_times_to_eat) // jesli ilosc zjedzonych posilkow jest wieksza rowna od wymaganej ilosci posilkow do zjedzenia...
			finished_eating++; // ... to inkrementujemy finished_eating, ktory liczy filozofow, ktorzy zjedli wyamagana ilosc posilkow
		pthread_mutex_unlock(philos[x].meal_lock);
		x++;
	}
	if(finished_eating == philos[0].num_of_philos) // jezeli ilosc filozofow ktorzy zjedli rowna sie calkowitej liczbie filozofow
	{
		pthread_mutex_lock(philos[0].dead_lock);
		*philos->dead = 1; // to ustawiam flage "dead" na 1
		pthread_mutex_unlock(philos[0].dead_lock);
		return(1);
	}
	return(0);
}

// Funkcja monitoruje stan filozofow w czasie rzeczywistym. W nieskonczonej petli sprawdza 2 warunki:
// czy ktorys z filozofow umarl oraz czy wszyscy filozofowie zjedli. Jesli ktorys z tych warunkow
// zostanie spelniony, to petla sie konczy, a funkcja zwraca wskaznik, ktory jej przekazano.

void	*monitor(void *pointer)
{
	t_philo	*philos;

	philos = (t_philo *)pointer; // "pointer" jest castowany na "t_philo *" i przypisywany do zmienne "philos"
	while(1) // to jest nieskonczona petla, ktora wykonuje sie dopoki nie zostanie spelniony ktorys warunek z ifa
	{
		if(check_if_any_dead(philos) == 1 || check_if_all_ate(philos) == 1) // sprawdzamy czy ktorys z filozofow umarl lub czy wszyscy zjedli; 1 oznacza prawde, czyli spelniony warunek
			break ; // jesli ktorys z powyzszych warunkow jest spelniony to przerywamy dzialanie petli i wychodzimy z niej
	}
	return(pointer); // zwracamy pointer
}