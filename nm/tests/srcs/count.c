#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int banane()
{
	return (strlen("banane"));
}

int main(int ac, char **av)
{
	if (ac != 2)
	{
		puts("usage : count string");
	}	
	else
	{
		printf("%lu\n", strlen(av[1]));
	}
	return (0);
}
