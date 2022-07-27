#ifndef inlcudes_H
#define inlcudes_H
#pragma once
#define NOMINMAX

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <math.h> //included in fstream/iostream/sstream/string but standard practice is to not rely on that
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <stdio.h>
#include <filesystem>

int pvp();

struct weapon
{
	std::string wname = "";
	int wcatt = 0;
};
struct armor
{
	std::string aname = "";
	int amchp = 0;
	int aregen = 0;
	int atotalstat = 0;
};
struct helmet
{
	std::string hname = "";
	int hmchp = 0;
	int hregen = 0;
	int htotalstat = 0;
};

namespace deadlibrary
{
	class dlclass
	{
	public:
		static void swapweapon(weapon& i, weapon& j);
		static void swaparmor(armor& i, armor& j);
		static void swaphelmet(helmet& i, helmet& j);
	};
}

#pragma comment(lib, "Winmm.lib")
#endif