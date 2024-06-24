/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrechuli <mrechuli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 18:40:20 by mrechuli          #+#    #+#             */
/*   Updated: 2024/06/24 18:40:22 by mrechuli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// sprawdzam czy wprowadzone dane sa liczbami, jesli nie to zwracam blad (1)

int	ft_isnumber(char *arg)
{
	int x;

	x = 0;
	while(arg[x])
	{
		if(arg[x] < '0' || arg[x] > '9')
			return(1); // 1 to kod bledu (co oznacza ze to nie jest liczba)
		x++;
	}
	return(0); // to znaczy ze nie ma bledu (czyli ze to jest liczba)
}

// sprawdza czy wprowadzone dane sa prawidlowe
// ogranicza sie to wlasciwie tylko do sprawdzenia czy to liczba i czy nie jest ujemna
// dodatkowo w pierwszym argumencie sprawdzam czy nie przekracza wartosci PHILO_MAX
// dodatkowo w piatym argumencie sprawdzam czy on wogole istnieje (bo to jest opcjonalny argument)

int	check_valid_args(char *argv[])
{
	if((ft_atol(argv[1]) > PHILO_MAX) || (ft_atol(argv[1]) <= 0) || (ft_isnumber(argv[1]) == 1)) // sprawdza czy ilosc filozofow jest OK
		return(write(2, "Invalid philosophers number\n", 29), 1);
	if(ft_atol(argv[2]) <= 0 || ft_isnumber(argv[2]) == 1)
		return(write(2, "Invalid time to die\n", 21), 1);
	if(ft_atol(argv[3]) <= 0 || ft_isnumber(argv[3]) == 1)
		return(write(2, "Invalid time to eat\n", 21), 1);
	if(ft_atol(argv[4]) <= 0 || ft_isnumber(argv[4]) == 1)
		return(write(2, "Invalid time to sleep\n", 23), 1);
	if(argv[5] && (ft_atol(argv[5]) < 0 || ft_isnumber(argv[5]) == 1))
		return(write(2, "Invalid numbers of times each philosopher must eat\n", 51), 1);
	return(0);
}

// struktura wprowadza danych:
// ./a.out 5 800 200 200 7
// 5 - liczba filozofow
// 800 - czas po ktorym filozof umrze jesli nie zje
// 200 - czas jaki potrzebuje filozof do zjedzenia
// 200 - czas jaki potrzebuje filozof do snu
// 7 - ilosc posilkow, jakie wszyscy filozofowie musza zjesc przed zakonczeniem programu (* argument opcjonalny)
// !!! CZAS PODAWANY JEST W MILISEKUNDACH !!! (1 sekunda = 1000 milisekund = 1000000 mikrosekund)
// PHILO_MAX to stala, ktora okresla maksymalna ilosc filozofow w programie; jest zdefiniowana w  philo.h

int main(int argc, char *argv[])
{
	t_program		program;
	t_philo			philos[PHILO_MAX]; // "philos" to tablica struktur "t_philo", ktora przechowuje informacje o kazdym filozofie; rozmiar tej tablicy to "PHILO_MAX", co oznacza, ze moze przechowywac info o maksymalnie "PHILO_MAX" filozofach
	pthread_mutex_t	forks[PHILO_MAX]; // tablica mutexow, ktora reprezentuje widelce. Kazdy filozof ma prawy i lewy wideleec, a tablica "forks" zawiera mutexy dla kazdego widelca

	if(argc != 5 && argc != 6) // jesli liczba argumentow nie jest rowna ani 5 ani 6...
	{
		write(2, "Wrong argument count\n", 22); // ... to wypisuje komunikat bledu (2 odpowiada standardowemu wyjsciu bledu)
		return(1); // i koncze program z wartoscia 1 (bo 1 standardowo oznacza to blad programu)
	}
	if(check_valid_args(argv) == 1) // jesli funkcja zwraca 1 oznacza to ze podane argumenty wejsciowe sa nieprawidlowe
		return(1); // wiec zwracam 1, bo jest to standardowy kod bledu
	init_program(&program, philos); // inicjalizuje strukture "program", ktora zawiera globalne informacje o stanie programu i przekazuje tablice filozfow; "&program" oznacza adres zmiennej "program", piszac &program przekazuje wskaznik do tej struktury; "philos" jest tablica struktur, nazwa tablicy w C jest wskaznikiem do jej pierwszego elementu
	init_forks(forks, ft_atol(argv[1])); // inicjalizuje mutexy widelcow
	init_philos(philos, &program, forks, argv); // inicjalizuje filozofow, ustawiajac wszystkie niezbedne parametry; funkcja ta wykorzystuje tablice "philos", strukture "program" oraz tablice "forks"
	thread_create(&program, forks); // tworzy watki dla kazdego filozofa
	destroy_all(NULL, &program, forks); // NULL, bo nie mam zadnego komunikatu, ktory chce wyswietlic. A nie wyswietlam komunikatu, bo "niszcze" wszystko NIE z powodu bledu.
	return(0);
}