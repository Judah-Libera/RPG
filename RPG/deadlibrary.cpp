#include "includes.h"
//#include "Source.cpp"

namespace deadlibrary
{
	void func::swapweapon(weapon &i, weapon &j)
	{
		weapon t = i;
		i = j;
		j = t;
	}
	void func::swaparmor(armor &i, armor &j)
	{
		armor t = i;
		i = j;
		j = t;
	}
	void func::swaphelmet(helmet &i, helmet &j)
	{
		helmet t = i;
		i = j;
		j = t;
	}
}