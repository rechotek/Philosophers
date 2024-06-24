/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrechuli <mrechuli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 18:40:54 by mrechuli          #+#    #+#             */
/*   Updated: 2024/06/24 18:40:55 by mrechuli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// Funkcja dead_loop ma na celu sprawdzenie, czy wątek filozofa powinien zakończyć swoje działanie.
// Robi to poprzez sprawdzenie wartości zmiennej dead, która jest współdzielona między wszystkimi wątkami filozofów,
// a jej dostęp jest synchronizowany przy użyciu mutexu dead_lock.
// Zwroc uwage ze jak dead == 1 to odblokowujemy mutex i zwracamy 1 co oznacza ze watek powinien byc ZAKONCZONY (bo wczesniej program lub inny watek oznaczyl dead jako 1, czyli ze zakonczyl swoje dzialanie)
// a jak dead != 1 to tez odblokowujemy mutex, ale zwracamy 0, co oznacza ze watek powinien KONTYNUOWAC swoje dzialanie
// Odblokowanie mutexu dead_lock w obu przypadkach (*philo->dead == 1 oraz *philo->dead != 1) jest konieczne, aby zapewnić
// bezpieczny dostęp do zmiennej dead w programowaniu wielowątkowym. Blokowanie i odblokowywanie mutexu w odpowiednich
//  miejscach zapewnia synchronizację i uniknięcie problemów z dostępem do współdzielonych zasobów między wątkami.
// dead == 0 oznacza to ze dany watek wykonuje dzialanie
// dead == 1 oznacza ze dany watek zakonczyl dzialanie

int	dead_loop(t_philo *philo)
{
	pthread_mutex_lock(philo->dead_lock); // Funkcja zaczyna od zablokowania mutexu dead_lock. Blokowanie mutexu zapewnia, że dostęp do współdzielonej zmiennej "dead" jest chroniony przed równoczesnym dostępem przez inne wątki, zapobiegając w ten sposób błędom synchronizacji.
	if(*philo->dead == 1) // dead == 1 oznacza, ze inny watek (lub glowny program) ustawil "dead" na 1, co sygnalizuje filozofowi, ze powinien zakonczyc swoje dzialanie
	{
		pthread_mutex_unlock(philo->dead_lock); // odblokowuje mutex dead_lock
		return(1); // zwracam 1, co sygnalizuje ze watek powinien zakonczyc swoje dzialanie
	}
	pthread_mutex_unlock(philo->dead_lock); // Jeśli wartość "dead" nie wynosi 1, mutex dead_lock jest odblokowywany, pozwalając innym wątkom na dostęp do zmiennej dead.
	return(0); // zwraca 0 co oznacza ze watek powinien kontynuowac swoje dzialanie
}

// Jest tu opisana rutyna dzialania, ktora wykonuje kazdy filozof.

void	*philo_routine(void *pointer) // funkcja przyjmuje argument "pointer", ktory jest wskaznikiem typu "void *". Jest to standardowa sygnatura funkcji uzywanej do tworzenia watkow w C.
{
	t_philo	*philo; 

	philo = (t_philo *)pointer; // pointer jest konwertowany na wskaznik typu "t_philo *", co oznacza, ze pointer jest rzutowany na wskaznik do struktury "t_philo". W ten sposob funkcja uzyskuje dostep do danych konkretnego filozofa.
	if(philo->id % 2 == 0) // jezeli "id" filozofa jest parzyste...
		ft_usleep(1); // (tu chyba mozna po prostu napisac usleep(1000) zamiast tworzyc nowa funkcje)... to usypiamy go na 1 milisekunde; robimy to aby unikac ryzyka zakleszczen, czyli nachodzenia na siebie watkow. Dzieki temu unikamy ryzyka, ze dwoch filozofow siegnie po ten sam widelec w tym samym czasie
	while(!dead_loop(philo)) // ta petla dziala dopoki filozof nie umrze (zyje=0, martwy=1), czyli dopoki "dead_loop" nie zwroci 1
	{
		eat(philo); // filozof probuje podniesc widelce (czyli zablokowac mutexy), zjesc, a nastpenie odlozyc widelce (odblokowac mutexy)
		dream(philo);
		think(philo);
	}
	return(pointer); // zwracana jest pointerm czyli oryginalny argument przekazany do funkcji. Jest to standradowa praktyka zwracania "void *" z funkcji watkowej w C
}

// Tworze watki. Kazdy filozof to jest oddzielny, jeden watek. Dodatkowo tworze "obserwatora"
// ktory bedzie odrebnym watkiem i ktory bedzie obserwowal prace filozofow.

int	thread_create(t_program *program, pthread_mutex_t *forks)
{
	pthread_t	observer; // to jest dodatkowy watek, ktory monitoruje prace filozofow
	int			x;

	if(pthread_create(&observer, NULL, &monitor, program->philos) != 0) // pthread_create tworzy watek obserwatora, ktory bedzie wykonywal funkcje "monitor" i przekaze ja jako argument "program->philos". Jesli utworzy go z sukcesem to zwraca 0. W kazdym innym przypadku jest blad.
		destroy_all("Thread creation error", program, forks); // Wiec jesli funkcja zwraca cos innego niz 1 to jest to blad i "niszcze" wszystko.
	x = 0;
	while(x < program->philos[0].num_of_philos) // petla wykonuje sie dopoki x jest mniejsze od ilosci filozofow (ta informacja jest przechowywana w philos[0].num_of_philos, czyli na pierwszym miejscu tablicy philos[0].num_of_philos)
	{
		if(pthread_create(&program->philos[x].thread, NULL, &philo_routine, &program->philos[x]) != 0) // tworzenie watku dla kazdego filozofa, ktore beda wykonywac funkcje "philo_routine"
			destroy_all("Thread creation error", program, forks); // ale jesli zwroci cos innego niz 0 (czyli blad) to "zniszczy wszystko i wypisze blad"
		x++;
	}
	x = 0;
	if(pthread_join(observer, NULL) != 0) // pthread_join dolacza watek obserwatora. Jesli dolaczy go z sukcesem to zwraca 0. W kazdym innym przypadku jest blad.
		destroy_all("Thread join error", program, forks); // Wiec jesli funkcja zwraca cos innego niz 1 to jest to blad i "niszcze" wszystko.
	while(x < program->philos[0].num_of_philos)  // petla wykonuje sie dopoki x jest mniejsze od ilosci filozofow
	{
		if(pthread_join(program->philos[x].thread, NULL) != 0) // oczekiwanie na ZAKONCZENIE watku dla kazdego filozofa. Funkcja pthread_join blokuje bieazy watek do momentu zakonczenia watku o identyfikatorze "thread"
			destroy_all("Thread join error", program, forks);
		x++;
	}
	return(0);
}