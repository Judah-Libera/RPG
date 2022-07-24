#include "includes.h"

namespace deadlibrary
{
	
	void dlclass::swapweapon(weapon &i, weapon &j)
	{
		weapon t = i;
		i = j;
		j = t;
	}
	void dlclass::swaparmor(armor &i, armor &j)
	{
		armor t = i;
		i = j;
		j = t;
	}
	void dlclass::swaphelmet(helmet &i, helmet &j)
	{
		helmet t = i;
		i = j;
		j = t;
	}
	
}
