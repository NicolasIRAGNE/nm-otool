int		g_global = 50;

int main(void)
{
	static int i = 0x55;
	int e;

	e = i;
	return (i - e);
}
