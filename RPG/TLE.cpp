// BUGS
// saving garbage values, somethign about them beign static

#include "includes.h"
#include <conio.h> // for _getch()

const char mylayna = 'L';
const char drayga = 'D';
const char grave = 'n';
const int MAP_SIZE_X = 20; // needs to match manual maps for them to work.
const int MAP_SIZE_Y = 39;

static int clvl = 0;
static char map[MAP_SIZE_X][MAP_SIZE_Y]; //x y // rows x columns // going down the matrix and going across the matrix
static int charx = 0;
static int chary = 0;
static char tileunderplayer = '@';

static int mylaynaintroduction = 0;
static int draygaintroduction = 0;
static int mylaynalater = 0;
static int draygalater = 0;
static int dungeonopen = 0;
static int queststarted = 0;
static int dungeoncomplete = 0;
static int draygadead = 0;
static int dungeonresult = 0; //1 - no drayga returned. 2 - drayga mangled returned. 3 - drayga alive returned.
static int connection = 0; //goes up to 3

using namespace std;

static void findC()
{
	for (int y = 0; y < MAP_SIZE_X; y++) // find player start position
		for (int x = 0; x < MAP_SIZE_Y; x++)
			if (map[x][y] == 'S')
			{
				charx = x;
				chary = y;
				map[x][y] = '#';
				break;
			}
}

static void printmap()
{
	tileunderplayer = map[charx][chary];
	map[charx][chary] = 'c';
	system("CLS");
	for (char i = 0; i < MAP_SIZE_X; i++)
	{
		for (char x = 0; x < MAP_SIZE_Y; x++)
			printf("%c", map[x][i]);
		printf("\n");
	}
	map[charx][chary] = tileunderplayer;
}

static void collisions(char tile)
{
	if (tile == mylayna)
		cout << "MYLAYNA!!!";
	else if (tile == drayga)
		cout << "DRAYGA!!!";
	else if (tile == grave)
	{
		if (draygadead == 1)
			cout << "rip drayga";
		else
			cout << "rip mylayna";
	}
}

static void questdungeon()
{
	cout << "dungeon";

	// include final interaction with mylayna. grave will be up on returned from this function

	dungeoncomplete = 1;
}

static void mylaynainteract()
{
	if (clvl < 10)
	{
		// talks about her love for drayga. and skill in alchemy (worries her tampering with forces better left alone would make fate hate her(curse)???).

		mylaynaintroduction = 1;
	}

	if (clvl >= 10 || clvl < 15)
	{
		if (mylaynaintroduction == 0)
		{
			// just friendly greeting
		}
		else
		{
			// says drayga went chasing a lgened sayign it was the only that could be worthy enough of his intentions.
		}
		connection++;
	}

	if (clvl >= 15 || clvl < 20)
	{
		if (mylaynaintroduction == 0)
		{
			// happy greeting
		}
		else
		{
			// proposed gonna live the bestest life everythign is perfect
		}
		connection++;
	}

	if (clvl >= 20 || clvl < 25)
	{
		if (mylaynaintroduction == 0)
		{
			// ignores you focusing on her work
		}
		else
		{
			// hes not feeling well but its ok i'll figure it out, i always do. he needs em to. i know its bad and gonna get worse, theres somethign hes not tellign me but i can help i know i can.
		}
		mylaynalater = 1;
	}

	if (clvl >= 25 || clvl < 30)
	{
		if (mylaynaintroduction == 0 || mylaynalater == 0 || draygalater == 0 || connection < 2)
		{
			// crying
		}
		else
		{
			// hes gone. he left. the potions didn't work and he left. don't know where but he mentioned a fate twister, somethign  that could break a curse. he promised he would be back soon but its been days, i can't loose him.
		}
		dungeonopen = 1;
	}

	if (clvl > 30)
	{
		if (mylaynaintroduction == 0)
		{
			// empty gaze with a lifeless hello
		}
		else
		{
			// he must have run off to save me from whatever he feared the curse would do to him. hes gone now
		}
		dungeonopen = 0;
	}
}

static void draygainteract()
{
	if (clvl < 10)
	{
		//talks about his love for mylayna. and skill as blacksmith/adventurer (wants to do somethign worth while with it(gem)???).

		draygaintroduction = 1;
	}

	if (clvl >= 15 || clvl < 20)
	{
		if (draygaintroduction == 0)
		{
			// friendly greeting
		}
		else
		{
			//needed that gem, didn't know if it was real but it was worth a shot, its perfect. proposed gonna live the bestest life everythign is perfect. feeling a bit odd though, probably jsut exhuasted from the way he had to fight with the gem.
		}
		connection++;
	}

	if (clvl >= 20 || clvl < 25)
	{
		if (draygaintroduction == 0)
		{
			// worried and stressed greeting
		}
		else
		{
			// the gem must have been cursed. feeling worse, not loosing strength yet but can feel his mind slipping and dreams of aweful things (stalked by abeast bent on revenge)
		}
		draygalater = 1;
	}
}

void TLE(int charlvl)
{
	ifstream data;
	string line;
	data.open("twilightsedge/qpd.txt");
	getline(data, line);
	istringstream in(line);
	//in >> mylaynaintroduction >> draygaintroduction >> mylaynalater >> draygalater >> dungeonopen >> queststarted >> dungeoncomplete >> draygadead >> dungeonresult >> connection;

	cout << mylaynaintroduction << draygaintroduction;
	system("pause");

	clvl = charlvl;

	if (draygaintroduction == 1 && mylaynaintroduction == 1)
		queststarted = 1;


	ifstream is("twilightsedge/map.txt");
	for (int y = 0; y < MAP_SIZE_X; y++)
		for (int x = 0; x < MAP_SIZE_Y; x++)
			map[x][y] = is.get();
	findC();
	if (dungeonopen == 1)
	{
		map[MAP_SIZE_Y - 2][MAP_SIZE_X - 4] = '#';
		map[MAP_SIZE_Y - 2][MAP_SIZE_X - 5] = '#';
	}


	if (clvl >= 10 && clvl < 15)
	{
		map[13][15] = '0';
	}

	if (clvl >= 15 && clvl < 20)
	{
		map[13][15] = 'D';
	}

	if (clvl >= 25 && clvl < 30)
	{
		map[13][15] = '0';
	}

	if (dungeoncomplete == 1)
	{
		map[1][13] = 'n';
		if (dungeonresult == 1)
		{
			draygadead = 1;
		}
		else if (dungeonresult == 2)
		{
			draygadead = 1;
		}
		else
		{
			map[2][12] = 'M';
		}
	}
	
	printmap();
	int input = 0;
	while (1)
	{
		input = 0;
		switch (input = _getch())
		{
		case KEY_UP:
			if (map[charx][chary - 1] != '@' && map[charx][chary - 1] != ' ')
				chary--;
			break;
		case KEY_DOWN:
			if (map[charx][chary + 1] != '@' && map[charx][chary + 1] != ' ')
				chary++;
			break;
		case KEY_LEFT:
			if (map[charx - 1][chary] != '@' && map[charx - 1][chary] != ' ')
				charx--;
			break;
		case KEY_RIGHT:
			if (map[charx + 1][chary] != '@' && map[charx + 1][chary] != ' ' && map[charx + 1][chary] != ']')
				charx++;
			break;
		default:
			break;
		}
		printmap();
		if (chary >= 0 && charx < MAP_SIZE_Y - 1)
			collisions(map[charx][chary]);
		else
		{
			system("CLS");
			if (chary < 0)
			{
				ofstream out("twilightsedge/qpd.txt");
				out << mylaynaintroduction << " " << draygaintroduction << " " << mylaynalater << " " << draygalater << " " << dungeonopen << " " << queststarted << " " << dungeoncomplete << " " << draygadead << " " << dungeonresult << " " << connection << "\n";
				return;
			}
			else
				questdungeon();
		}
		cout << charx;
		cout << chary;
	}
}