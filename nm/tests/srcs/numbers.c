#include <stdlib.h>
#include <stdio.h>

int main()
{
	char *str[] = {"1", "2", "10", "1400", "123456789", "-1", "salut"};

	int i = 0;
	while (i < sizeof(*str) - 1)
	{
		printf("%d\n", atoi(str[i]));
		i++;
	}
}
