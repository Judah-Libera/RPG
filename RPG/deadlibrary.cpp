#include "includes.h"

namespace deadlibrary
{
	void swapweapon(weapon &i, weapon &j)
	{
		weapon t = i;
		i = j;
		j = t;
	}
	void swaparmor(armor &i, armor &j)
	{
		armor t = i;
		i = j;
		j = t;
	}
	void swaphelmet(helmet &i, helmet &j)
	{
		helmet t = i;
		i = j;
		j = t;
	}

	void numswap(int *a, int *b)
	{
		int c = *a;
		*a = *b;
		*b = c;
	}

	void strswap(std::string *a, std::string *b)
	{
		std::string c = *a;
		*a = *b;
		*b = c;
	}
}
