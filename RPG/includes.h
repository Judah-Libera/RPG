#ifndef inlcudes_H
#define inlcudes_H
#pragma once
#pragma warning(disable: 4996) // VS has another function it likes more
#pragma warning(disable: 6031) // don't check func return for failure
#pragma warning(disable: 4244) // int set equal to double
#pragma warning(disable: 4834) // ignored function returnvvalue
#define NOMINMAX

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

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
struct character
{
	std::string cclass = "";
	std::string name = "";
	std::string nametitle = "";
	int chp = 0;
	int mchp = 0;
	double catt = 0;
	int attlvlups = 0;
	double regen = 0;
	int cxp = 0;
	int lvl = 0;
	int score = 0;
	int dif = 1;
	weapon we = { "", 0 }; //weapon equiped
	armor ae = { "", 0, 0, 0 };
	helmet he = { "", 0, 0, 0 };
};

//int pvp();
int dungeon(int dungeondiff, int dungeontype, character& c, weapon weaponsarray[], armor armorsarry[], helmet helmetsarray[]);
int TLE(int charlvl, character& c, weapon weaponsarray[], armor armorsarray[], helmet helmetsarray[]);

namespace deadlibrary
{
	void numswap(int *a, int *b);
	void strswap(std::string *a, std::string *b);

	int rng(int min, int max);
	int rngnd(int min, int max, double precision);
}

namespace rpglib
{
	void sortweapon(weapon arr[5]);
	void sortarmor(armor arr[5]);
	void sorthelmet(helmet arr[5]);

	void swapweapon(weapon& i, weapon& j);
	void swaparmor(armor& i, armor& j);
	void swaphelmet(helmet& i, helmet& j);

	void volumeupdate(int mvolume, int sfxvolume);
}

#pragma comment(lib, "Winmm.lib")
#endif