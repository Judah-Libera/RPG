#include "includes.h"

//remove try catch blocks once fixed

namespace deadlibrary
{
	
	void dlclass::swapweapon(weapon &i, weapon &j)
	{
		if (i.wname.empty() || j.wname.empty() || i.wcatt == NULL || j.wcatt == NULL) // null safegaurd
		{
			std::cout << "uninitialized item.\n";
			return;
		}
		try // hopefully runs even if it breaks;
		{
			weapon t = i;
			i = j;
			j = t;
		}
		catch (const std::exception& e)
		{
			std::cout << "exception";
		}
	}
	void dlclass::swaparmor(armor &i, armor &j)
	{
		if (i.aname.empty() || j.aname.empty() || i.amchp == NULL || j.amchp == NULL || i.aregen == NULL || j.aregen == NULL || i.atotalstat == NULL || j.atotalstat == NULL)
		{
			std::cout << "uninitialized item.\n";
			return;
		}
		try 
		{
		armor t = i;
		i = j;
		j = t;
		}
		catch (const std::exception& e)
		{
			std::cout << "exception";
		}
	}
	void dlclass::swaphelmet(helmet &i, helmet &j)
	{
		if (i.hname.empty() || j.hname.empty() || i.hmchp == NULL || j.hmchp == NULL || i.hregen == NULL || j.hregen == NULL || i.htotalstat == NULL || j.htotalstat == NULL)
		{
			std::cout << "uninitialized item.\n";
			return;
		}
		try
		{
		helmet t = i;
		i = j;
		j = t;
		}
		catch (const std::exception& e)
		{
			std::cout << "exception";
		}
	}
	
}
