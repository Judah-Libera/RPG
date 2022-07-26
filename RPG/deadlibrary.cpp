#include "includes.h"

//remove try catch blocks once fixed

namespace deadlibrary
{
	
	void dlclass::swapweapon(weapon &i, weapon &j)
	{
		/*if (i.wname.empty() || j.wname.empty() || i.wcatt == NULL || j.wcatt == NULL) // null safegaurd, except default constructor it thinks is null
		{
			//std::cout << "uninitialized item.\n";


			temp details for debugging
			if (i.wname.empty())
			{
				std::cout << 1;
			}
			if (j.wname.empty())
			{
				std::cout << 2;
			}
			if (i.wcatt == NULL)
			{
				std::cout << 3;
			}
			if (j.wcatt == NULL)
			{
				std::cout << 4;
			}


			system("pause");
			return;
		}*/
		try // hopefully runs even if it breaks;
		{
			weapon t = i;
			i = j;
			j = t;
		}
		catch (const std::exception& e)
		{
			std::cout << "exception";
			system("pause");
		}
	}
	void dlclass::swaparmor(armor &i, armor &j)
	{
		/*if (i.aname.empty() || j.aname.empty() || i.amchp == NULL || j.amchp == NULL || i.aregen == NULL || j.aregen == NULL || i.atotalstat == NULL || j.atotalstat == NULL)
		{
			std::cout << "uninitialized item.\n";
			system("pause");
			return;
		}*/
		try //doesn'st see mto do wanything
		{
		armor t = i;
		i = j;
		j = t;
		}
		catch (const std::exception& e)
		{
			std::cout << "exception";
			system("pause");
		}
	}
	void dlclass::swaphelmet(helmet &i, helmet &j)
	{
		/*if (i.hname.empty() || j.hname.empty() || i.hmchp == NULL || j.hmchp == NULL || i.hregen == NULL || j.hregen == NULL || i.htotalstat == NULL || j.htotalstat == NULL)
		{
			std::cout << "uninitialized item.\n";
			system("pause");
			return;
		}*/
		try
		{
		helmet t = i;
		i = j;
		j = t;
		}
		catch (const std::exception& e)
		{
			std::cout << "exception";
			system("pause");
		}
	}
	
}
