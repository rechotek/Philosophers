#include "philo.h"

int	ft_atol(char *str)
{
	unsigned long long	num;
	int					sign;
	int					x;

	num = 0;
	sign = 1;
	x = 0;
	while(str[x] && (str[x] == ' ' || str[x] == '\r' || str[x] == '\t' || str[x] == '\v' || str[x] == '\f' || str[x] == '\n'))
		x++;
	if(str[x] == '-' || str[x] == '+')
	{
		if(str[x] == '-')
			sign = -1;
		x++;
	}
	while(str[x] >= '0' && str[x] <= '9')
	{
		num = (num * 10) + (str[x] - '0');
		x++;
	}
	return(sign * num);
}