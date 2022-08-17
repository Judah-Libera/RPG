#include "includes.h"

namespace deadlibrary
{
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


	int rng(int min, int max)
	{
		return (rand() % (max - min + 1) + min);
	}
	int rngnd(int min, int max, double precision) // applies 2 wieghts that add to 1. each wieght goes to its own random number. the first number is made by taking the average of 4 rng's, thus sticking to a tight normal distribution. the second number is just a normal rng. thus precision gets anywhere between being completely based on rng or completely based off of normal.
	{
		if (precision > 1 || precision < 0) //make sure precision is in bound
			precision = .5;
		if (min > max) // make sure min max are in order
			numswap(&min, &max);
		double returnnumber = 0;
		int range = (max - min) + 1;
		for (int i = 0; i < 4; i++) // first number
			returnnumber += ((double)(rand() % range) + min) / 4;
		returnnumber = returnnumber * precision; //wight to first number
		returnnumber = returnnumber + (((double)(rand() % range) + min) * (1 - precision)); //second number and its weight
		return round(returnnumber);
	}
}
