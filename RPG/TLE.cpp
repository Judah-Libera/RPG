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
static int dungeonopen = 1; // should be 0. is 1 for testing is dungeon call from tle works.
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
		mylaynainteract();
	else if (tile == drayga)
		draygainteract();
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
		cout << "dialogue expressing a liking of drayga. also is skilled in alchemy, and tends to loose herself in experimenting but knows some things are better left alone despite her not doing so. fate can be a cruel mistress." << endl;
		mylaynaintroduction = 1;
	}

	if (clvl >= 10 || clvl < 15)
	{
		if (mylaynaintroduction == 0)
		{
			cout << "sup" << endl;
		}
		else
		{
			cout << "drayga sadi he had a surprise for me then just left. not really sure where he went or why, but i hope its worth the wait." << endl;
		}
		connection++;
	}

	if (clvl >= 15 || clvl < 20)
	{
		if (mylaynaintroduction == 0)
		{
			cout << "sup *exitedly*" << endl;
		}
		else
		{
			cout << "drayga proposed, wich ig is cool" << endl;
		}
		connection++;
	}

	if (clvl >= 20 || clvl < 25)
	{
		if (mylaynaintroduction == 0)
		{
			cout << "don't have time to talk rn" << endl;
		}
		else
		{
			cout << "draygas feeling aweful and it's getting worse. He says he's find but i know he's lying. i don't know how to help him yet but i'll figure it out." << endl;
		}
		mylaynalater = 1;
	}

	if (clvl >= 25 || clvl < 30)
	{
		if (mylaynaintroduction == 0 || mylaynalater == 0 || draygalater == 0 || connection < 2)
		{
			cout << "*crying*" << endl;
		}
		else
		{ 
			cout << "he left. nothign i tried worked and he gave up on me, as he should have. He mentioned knowing where i could find something to break the curse. He promised he'd be back soon but its been days. I can't loose him." << endl;
		}
		dungeonopen = 1;
	}

	if (clvl > 30)
	{
		if (mylaynaintroduction == 0)
		{
			cout << "her empty gaze is staring somewhere into the distance. her lifeless eyes barely turn to meet yours as she greets you with a fient \"hello\" you as if you were only part of a fading imagination." << endl;
		}
		else
		{
			cout << "whatever he thought was happening he must have been right. He left me. Left to save me from whatever the cursed had done to him. But i would have rathered he take me with him anyways. I don't know what to do anymore." << endl;
		}
		dungeonopen = 0;
	}
}

static void draygainteract()
{
	if (clvl < 10)
	{
		cout << "character intro for drayga. like mylayna, is blacksmith, does a bit of legend chasing too." << endl;

		draygaintroduction = 1;
	}

	if (clvl >= 15 || clvl < 20)
	{
		if (draygaintroduction == 0)
		{
			cout << "sup" << endl;
		}
		else
		{
			cout << "got it. didn't even know if it was real but i had to take the chamce. it's perfecy, she loved it. it was exhausting but worth it. like really exhausting. im usually totally recovered y now but im still not feeling back to my usual self." << endl;
		}
		connection++;
	}

	if (clvl >= 20 || clvl < 25)
	{
		if (draygaintroduction == 0)
		{
			cout << "sup *worried/stressed*" << endl;
		}
		else
		{
			cout << "I'm loosing myself. my strecnht is leaving, my mind spends half its time blank and my dreams have been getting worse and worse. Mylayna thinks im cursed and shes' prbably right. as skilled as she is though i don't think its something she'll be able to help with. don't tell her though, shes worried enough as is. figures it would end up like this, thigns were going too well." << endl;
		}
		draygalater = 1;
	}
}

int TLE(int charlvl, character& c, weapon weaponsarray[], armor armorsarray[], helmet helmetsarray[])
{
	ifstream data;
	string line;
	data.open("twilightsedge/qpd.txt");
	getline(data, line);
	istringstream in(line);
	in >> mylaynaintroduction >> draygaintroduction >> mylaynalater >> draygalater >> dungeonopen >> queststarted >> dungeoncomplete >> draygadead >> dungeonresult >> connection;

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
				return 0;
			}
			else
			{
				dungeonresult = dungeon(c.lvl, 2, c, weaponsarray, armorsarray, helmetsarray); //return 0 if dungeon was exited, return 1 if player died. int parameter 2 is dungeontype so it uses TEmap
				if (dungeonresult == 1)
					return 1; //return 1 if player died
			}
		}
		cout << charx;
		cout << chary;
	}
}