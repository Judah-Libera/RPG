#include "includes.h"

namespace deadlibrary
{
	void sortweapon(weapon arr[5])
	{
		for (int i = 0; i < 5 - 1; i++) // sort by descending power
			for (int j = 0; j < 5 - i - 1; j++)
				if (arr[j].wcatt < arr[j + 1].wcatt)
					swapweapon(arr[j], arr[j + 1]);
	}
	void sortarmor(armor arr[5])
	{
		for (int i = 0; i < 5 - 1; i++) // sort by descending power
			for (int j = 0; j < 5 - i - 1; j++)
				if (arr[j].atotalstat < arr[j + 1].atotalstat)
					swaparmor(arr[j], arr[j + 1]);
	}
	void sorthelmet(helmet arr[5])
	{
		for (int i = 0; i < 5 - 1; i++) // sort by descending power
			for (int j = 0; j < 5 - i - 1; j++)
				if (arr[j].htotalstat < arr[j + 1].htotalstat)
					swaphelmet(arr[j], arr[j + 1]);
	}

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


	int intnormaldistribution(int min, int max, double precision) // applies 2 wieghts that add to 1. each wieght goes to its own random number. the first number is made by taking the average of 4 rng's, thus sticking to a tight normal distribution. the second number is just a normal rng. thus precision gets anywhere between being completely based on rng or completely based off of normal.
	{
		if (precision > 1 || precision < 0) //make sure precision is in bound
			precision = .5;
		if (min > max) // make sure min max are in order
			numswap(&min, &max);
		double returnnumber = 0;
		int range = (max - min) + 1;
		for (int i = 0; i < 4; i++) // first number
			returnnumber += (double)(rand() % range + min) / 4;
		returnnumber = returnnumber * precision; //wight to first number
		returnnumber = returnnumber + ((rand() % range + min) * (1 - precision)); //second number and its weight
		return round(returnnumber);
	}
}
