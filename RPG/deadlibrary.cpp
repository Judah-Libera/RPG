#include "includes.h"

namespace deadlibrary
{
	
	void dlclass::swapweapon(weapon &i, weapon &j)
	{
		if (i.wname.empty() || j.wname.empty() || i.wcatt == NULL || j.wcatt == NULL) // null safegaurd
		{
			std::cout << "uninitialized item.\n";
			return;
		}
		weapon t = i;
		i = j;
		j = t;
	}
	void dlclass::swaparmor(armor &i, armor &j)
	{
		if (i.aname.empty() || j.aname.empty() || i.amchp == NULL || j.amchp == NULL || i.aregen == NULL || j.aregen == NULL || i.atotalstat == NULL || j.atotalstat == NULL)
		{
			std::cout << "uninitialized item.\n";
			return;
		}
		armor t = i;
		i = j;
		j = t;
	}
	void dlclass::swaphelmet(helmet &i, helmet &j)
	{
		if (i.hname.empty() || j.hname.empty() || i.hmchp == NULL || j.hmchp == NULL || i.hregen == NULL || j.hregen == NULL || i.htotalstat == NULL || j.htotalstat == NULL)
		{
			std::cout << "uninitialized item.\n";
			return;
		}
		helmet t = i;
		i = j;
		j = t;
	}
	
}
