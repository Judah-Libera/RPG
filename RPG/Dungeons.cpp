#include "includes.h"
#include <conio.h> // for _getchar().

//BUGS

//FEATURES
//make enemies move around
//limit access? like half of item lvl 9 or 10 drops are instead dungeon key. or given a dungeon key every x levels
//gurentee there is exit or all boss within available pathing (don't like spawn exit next to player)
//add soundtracks
//have a 4th drop item of hp pot. can make it stay with chara after dungeon later

const int MAP_SIZE_X = 20; // needs to match manual maps for them to work.
const int MAP_SIZE_Y = 20;
const char PLAYER = 'C';
const char BOSS = 'B';
const char ITEM = 'T';
const char EXIT = 'O';
const char ENEMY = 'M';
const char DRAYGA = 'D';

using namespace std;

static void makeshape(int y, int x, int n);
static void findC();
static void initfog();
static void printmap();
static void updatefog();
static void itemdrop(character& c, weapon weaponsarray[], armor armorsarray[], helmet helmetsarray[]);
static int battle(character& c, weapon weaponsarray[], armor armorsarry[], helmet helmetsarray[]);
static void dungeonboss(character& c, weapon weaponsarray[], armor armorsarry[], helmet helmetsarray[]);
void exitdungeon();
static int bosscount();
static int checkcollision(character& c, weapon weaponsarray[], armor armorsarry[], helmet helmetsarray[]);
int dungeon(character& c, weapon weaponsarray[], armor armorsarry[], helmet helmetsarray[]);

static char map[MAP_SIZE_X][MAP_SIZE_Y]; //x y // rows x columns // going down the matrix and going across the matrix
static char fog[MAP_SIZE_X][MAP_SIZE_Y]; // ' ' is hidden, '.' is revealed
static int charx = 0;
static int chary = 0;
static char tileunderplayer = '@';
static int dungeondiff = 5; //stand in for character level in dungeon
static int charlevel = 0; //only needed for accuratly showing xp required for lvlup
static weapon wfound[10]; //for storing loot collected during dungeon
static int wfi = 0; //index for array
static armor afound[10];
static int afi = 0;
static helmet hfound[10];
static int hfi = 0;
static int xpfound;

static const double aiattscaler() { return (.035 * pow(dungeondiff, 2.0)) + (.1 * (double)dungeondiff) + 1; }
static const double aihpscaler() { return (.25 * pow(dungeondiff, 1.9)) + (2 * (double)dungeondiff) + 3; }
static const double xpgainscaler() { return ((double)((rand() % 3) + 2.0) * (1 + (double)dungeondiff / 2)); }
static const double xpcostscaler() { return ((.1 * (double)pow(charlevel, 2.0)) + (5 * (double)charlevel) + 5); }

static weapon createweapon()
{
	string wname; // total name, part1, part2
	string wname1;
	string wname2;
	int wnamei = (rand() % 4); //title selections
	int wnamei2 = (rand() % 4);
	int wcatt = ((rand() % 2) + 1 + dungeondiff * .4); // weapon power
	if (wcatt > 7) // change list titles are selected from based on power. better drops get better names
	{
		if (wnamei == 0)
			wname1 = "scythe of ";
		else if (wnamei == 1)
			wname1 = "Longsword of ";
		else if (wnamei == 2)
			wname1 = "Blades of ";
		else
			wname1 = "Battle-Axe of ";

		if (wnamei2 == 0)
			wname2 = "Darkness";
		else if (wnamei2 == 1)
			wname2 = "Nyx";
		else if (wnamei2 == 2)
			wname2 = "Blood";
		else
			wname2 = "Void";
		wname = wname1 + wname2;
	}
	else if (wcatt > 3)
	{
		if (wnamei == 0)
			wname1 = "Spear of ";
		else if (wnamei == 1)
			wname1 = "sword of ";
		else if (wnamei == 2)
			wname1 = "mace of ";
		else
			wname1 = "Flail of ";

		if (wnamei2 == 0)
			wname2 = "Destruction";
		else if (wnamei2 == 1)
			wname2 = "Brutality";
		else if (wnamei2 == 2)
			wname2 = "Anguish";
		else
			wname2 = "Vengeance";
		wname = wname1 + wname2;
	}
	else
	{
		if (wnamei == 0)
			wname1 = "torch of ";
		else if (wnamei == 1)
			wname1 = "dagger of ";
		else if (wnamei == 2)
			wname1 = "cane of ";
		else
			wname1 = "knife of ";

		if (wnamei2 == 0)
			wname2 = "Ash";
		else if (wnamei2 == 1)
			wname2 = "Doom";
		else if (wnamei2 == 2)
			wname2 = "Might";
		else
			wname2 = "Justice";
		wname = wname1 + wname2;
	}
	weapon w = { wname, wcatt }; // create weapon
	return w;
}

static armor createarmor()
{
	string aname; // works same as createweapon()
	string aname1;
	string aname2;
	int aname1i = (rand() % 2);
	int atotalstat = ((rand() % 2) + 1 + dungeondiff * .4); // total power to be distributed between hp and regen
	int bamchp = (rand() % (atotalstat + 1));
	int baregen = (atotalstat - bamchp);
	int amchp = bamchp * 5;
	int aregen = baregen * .7;
	if (amchp == 0 && aregen == 0)
		aregen = 1;
	if (atotalstat > 7)
	{
		if (aname1i == 0)
			aname1 = "Armor ";
		else
			aname1 = "Cuirass ";
		if ((float)bamchp / (float)atotalstat >= .7)
		{
			aname2 = "of Vitality";
		}
		else if ((float)bamchp / (float)atotalstat >= .4)
		{
			aname2 = "of Harmony";
		}
		else
		{
			aname2 = "of Youth";
		}
	}
	else if (atotalstat > 3)
	{
		if (aname1i == 0)
			aname1 = "Plate ";
		else
			aname1 = "Vestment ";
		if ((float)bamchp / (float)atotalstat >= .7)
		{
			aname2 = "of Life";
		}
		else if ((float)bamchp / (float)atotalstat >= .4)
		{
			aname2 = "of Equilibrium";
		}
		else
		{
			aname2 = "of Rejuvenation";
		}
	}
	else
	{
		if (aname1i == 0)
			aname1 = "Robe ";
		else
			aname1 = "Tunic ";
		if ((float)bamchp / (float)atotalstat >= .7)
		{
			aname2 = "of Health";
		}
		else if ((float)bamchp / (float)atotalstat >= .4)
		{
			aname2 = "of Balance";
		}
		else
		{
			aname2 = "of Recovery";
		}
	}
	aname = aname1 + aname2;
	armor a = { aname ,amchp, aregen, atotalstat };
	return a;
}

static helmet createhelmet()
{
	string hname; // worsk same as createweapon()
	string hname1;
	string hname2;
	int hname1i = (rand() % 2);
	int htotalstat = ((rand() % 2) + 1 + dungeondiff * .4); // total power to be distributed between hp and regen
	int bhmchp = (rand() % (htotalstat + 1));
	int bhregen = (htotalstat - bhmchp);
	int hmchp = bhmchp * 5;
	int hregen = bhregen * .7;
	if (hmchp == 0 && hregen == 0)
		hregen = 1;
	if (htotalstat > 7)
	{
		if (hname1i == 0)
			hname1 = "Crown ";
		else
			hname1 = "Helm ";
		if ((float)bhmchp / (float)htotalstat >= .7)
		{
			hname2 = "of Vitality";
		}
		else if ((float)bhmchp / (float)htotalstat >= .4)
		{
			hname2 = "of Harmony";
		}
		else
		{
			hname2 = "of youth";
		}
	}
	else if (htotalstat > 3)
	{
		if (hname1i == 0)
			hname1 = "Viel ";
		else
			hname1 = "Mask ";
		if ((float)bhmchp / (float)htotalstat >= .7)
		{
			hname2 = "of Life";
		}
		else if ((float)bhmchp / (float)htotalstat >= .4)
		{
			hname2 = "of Equilibrium";
		}
		else
		{
			hname2 = "of Rejuvenation";
		}
	}
	else
	{
		if (hname1i == 0)
			hname1 = "Circlet ";
		else
			hname1 = "Hood ";
		if ((float)bhmchp / (float)htotalstat >= .7)
		{
			hname2 = "of Health";
		}
		else if ((float)bhmchp / (float)htotalstat >= .4)
		{
			hname2 = "of Balance";
		}
		else
		{
			hname2 = "of Recovery";
		}
	}
	hname = hname1 + hname2;
	helmet h = { hname ,hmchp, hregen, htotalstat };
	return h;
}

static void makeshape(int y, int x, int n)
{
	switch (n)
	{
	case 0: // J
		map[x][y] = '@';
		map[x][y - 1] = '@';
		map[x][y + 1] = '@';
		map[x + 1][y] = '@';
		map[x + 2][y] = '@';
		map[x + 2][y - 1] = '@';
		break;
	case 1: // +
		map[x][y] = '@';
		map[x + 1][y] = '@';
		map[x - 1][y] = '@';
		map[x][y + 1] = '@';
		map[x][y - 1] = '@';
		break;
	case 2: // |
		map[x][y] = '@';
		map[x][y + 1] = '@';
		map[x][y + 2] = '@';
		map[x][y - 1] = '@';
		break;
	case 3: // -
		map[x][y] = '@';
		map[x + 1][y] = '@';
		map[x + 2][y] = '@';
		map[x - 1][y] = '@';
		break;
	case 4: // | |
		map[x - 1][y + 1] = '@';
		map[x - 1][y] = '@';
		map[x - 1][y - 1] = '@';
		map[x - 1][y - 2] = '@';
		map[x + 1][y + 1] = '@';
		map[x + 1][y] = '@';
		map[x + 1][y - 1] = '@';
		map[x + 1][y - 2] = '@';
		break;
	case 5: // =
		map[x + 1][y - 1] = '@';
		map[x][y - 1] = '@';
		map[x - 1][y - 1] = '@';
		map[x - 2][y - 1] = '@';
		map[x + 1][y + 1] = '@';
		map[x][y + 1] = '@';
		map[x - 1][y + 1] = '@';
		map[x - 2][y + 1];
		break;
	case 6: //L `|
		map[x - 1][y] = '@';
		map[x - 1][y - 1] = '@';
		map[x - 1][y + 1] = '@';
		map[x][y - 1] = '@';
		map[x + 2][y] = '@';
		map[x + 2][y + 1] = '@';
		map[x + 2][y + 2] = '@';
		map[x + 1][y + 2] = '@';
		break;
	default:
		break;
	}
}

static void makemap()
{
	for (int i = 0; i < MAP_SIZE_X; i++) //create map
		for (int x = 0; x < MAP_SIZE_Y; x++)
			map[x][i] = '#';

	for (int i = 0; i < MAP_SIZE_X; i++) //create border
	{
		map[MAP_SIZE_Y - 1][i] = '@';
		map[0][i] = '@';
	}
	for (int i = 1; i < MAP_SIZE_Y - 1; i++)
	{
		map[i][MAP_SIZE_X - 1] = '@';
		map[i][0] = '@';
	}

	int a = 4; // shape spawn density. should keep as a multiple of mapzixe x y respectively
	int b = 4;

	for (int i = 0; i < (int)(MAP_SIZE_Y / b); i++)
		for (int x = 0; x < (int)(MAP_SIZE_X / a); x++)
			makeshape(deadlibrary::rng(x * a, (x + 1) * a), deadlibrary::rng(i * b, (i + 1) * b), deadlibrary::rng(0, 6)); //first slot is for mapsizex

	int count = 0;
	for (int i = 0; i < MAP_SIZE_X; i++)
		for (int x = 0; x < MAP_SIZE_Y; x++)
		{
			if (map[x][i] == '#')
			{
				count = 0;
				if (map[x + 1][i] == '@')
					count++;
				if (map[x - 1][i] == '@')
					count++;
				if (map[x][i + 1] == '@')
					count++;
				if (map[x][i - 1] == '@')
					count++;

				if (deadlibrary::rng(0, 9) == 0) //10% enemy spawn
					map[x][i] = 'M';
				if (count == 2 || count == 3) // 50% on 2 touching tiles for item
					if (deadlibrary::rng(0, 2) == 0)
						map[x][i] = 'T';
				if (count == 3) // 50% any 3tile will exit, overriding item spawn
					if (deadlibrary::rng(0, 1) == 0)
						map[x][i] = 'O';
				if (count == 4) //fill in inaccesible tiels
					map[x][i] = '@';
				//    2 sides are walls (choke)                          other adjacent tile is not enemy or boss       other adjacent tile on other side                for wall on other side
				if (((map[x + 1][i] == '@' && map[x - 1][i] == '@') && (map[x][i + 1] != 'M' && map[x][i + 1] != 'B') && (map[x][i - 1] != 'M' && map[x][i - 1] != 'B')) || ((map[x][i + 1] == '@' && map[x][i - 1] == '@') && (map[x + 1][i] != 'M' && map[x + 1][i] != 'B') && (map[x - 1][i] != 'M' && map[x - 1][i] != 'B'))) //spawn enemy or boss at chokes. not if either of the two non wall are enemy or boss
					if (deadlibrary::rng(0, 2) == 0)
						map[x][i] = 'B';
					else
						map[x][i] = 'M';
			}
		}

	do // keep spawning player till they are on playable tile
	{
		charx = deadlibrary::rng(1, MAP_SIZE_X);
		chary = deadlibrary::rng(1, MAP_SIZE_Y);
	} while (map[charx][chary] != '#');

	/* // still possible to have no exit, but unlikely, and i'd like a better system. this allows for easy T grab and leave
	switch (deadlibrary::rng(0, 3)) //guerentee available exit
	{
	case 0: map[charx + 1][chary] = 'O';
		break;
	case 1: map[charx - 1][chary] = 'O';
		break;
	case 2: map[charx][chary + 1] = 'O';
		break;
	case 3: map[charx][chary - 1] = 'O';
		break;
	default:
		break;
	}
	*/
}

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

static void initfog()
{
	for (int y = 0; y < MAP_SIZE_X; y++) // create FOW
		for (int x = 0; x < MAP_SIZE_Y; x++)
			fog[x][y] = ' ';
}

static void printmap()
{
    tileunderplayer = map[charx][chary];
    map[charx][chary] = 'c';
	system("CLS");
    for (char i = 0; i < MAP_SIZE_X; i++)
    {
        for (char x = 0; x < MAP_SIZE_Y; x++)
        {
			if (fog[x][i] == ' ')
				printf(" ");
            else
            {
                printf("%c", map[x][i]);
            }
        }
		printf("\n");
    }
    map[charx][chary] = tileunderplayer;
}

static void updatefog()
{
    fog[charx + 1][chary] = '.';//normals
    fog[charx - 1][chary] = '.';
    fog[charx][chary + 1] = '.';
    fog[charx][chary - 1] = '.';

    if (map[charx + 1][chary] != '@' || map[charx][chary + 1] != '@') //diag must have at least one revealed nondiag path, stops walled off diags being revealed
        fog[charx + 1][chary + 1] = '.';//diagnols
    if (map[charx - 1][chary] != '@' || map[charx][chary + 1] != '@')
        fog[charx - 1][chary + 1] = '.';
    if (map[charx + 1][chary] != '@' || map[charx][chary - 1] != '@')
        fog[charx + 1][chary - 1] = '.';
    if (map[charx - 1][chary] != '@' || map[charx][chary - 1] != '@')
        fog[charx - 1][chary - 1] = '.';

    if (map[charx + 1][chary] != '@') //+1 vision in unblocked nondiags
        fog[charx + 2][chary] = '.';
    if (map[charx - 1][chary] != '@')
        fog[charx - 2][chary] = '.';
    if (map[charx][chary + 1] != '@')
        fog[charx][chary + 2] = '.';
    if (map[charx][chary - 1] != '@')
        fog[charx][chary - 2] = '.';
}

static void itemdrop(character& c, weapon weaponsarray[], armor armorsarray[], helmet helmetsarray[])
{
    int dt = (rand() % 3); // item drop type
    mciSendString(L"seek itemdsfx to start", NULL, 0, NULL);
    mciSendString(L"play itemdsfx", NULL, 0, NULL);
    if (dt == 0)
    {
        weapon wd = createweapon();
		wfound[wfi++] = wd;
        cout << "You've found " << wd.wname << "!" << endl;
        for (int i = 0; i < 5 - 1; i++) // sort inv by descending power
            for (int j = 0; j < 5 - i - 1; j++)
                if (weaponsarray[j].wcatt < weaponsarray[j + 1].wcatt)
					rpglib::swapweapon(weaponsarray[j], weaponsarray[j + 1]);
        if (wd.wcatt > weaponsarray[4].wcatt) // if dropped item is better than worse inventory item
            weaponsarray[4] = wd;
        for (int i = 0; i < 5 - 1; i++) // sort inv by descending power again with new item
            for (int j = 0; j < 5 - i - 1; j++)
                if (weaponsarray[j].wcatt < weaponsarray[j + 1].wcatt)
					rpglib::swapweapon(weaponsarray[j], weaponsarray[j + 1]);
    }
    if (dt == 1)
    {
        armor ad = createarmor();
		afound[afi++] = ad;
        cout << "You've found " << ad.aname << "!" << endl;
        for (int i = 0; i < 5 - 1; i++)
            for (int j = 0; j < 5 - i - 1; j++)
                if (armorsarray[j].atotalstat < armorsarray[j + 1].atotalstat)
					rpglib::swaparmor(armorsarray[j], armorsarray[j + 1]);
        if (ad.atotalstat > armorsarray[4].atotalstat)
            armorsarray[4] = ad;
        for (int i = 0; i < 5 - 1; i++)
            for (int j = 0; j < 5 - i - 1; j++)
                if (armorsarray[j].atotalstat < armorsarray[j + 1].atotalstat)
					rpglib::swaparmor(armorsarray[j], armorsarray[j + 1]);
    }
    if (dt == 2)
    {
        helmet hd = createhelmet();
		hfound[hfi++] = hd;
        cout << "You've found " << hd.hname << "!" << endl;
        for (int i = 0; i < 5 - 1; i++)
            for (int j = 0; j < 5 - i - 1; j++)
                if (helmetsarray[j].htotalstat < helmetsarray[j + 1].htotalstat)
					rpglib::swaphelmet(helmetsarray[j], helmetsarray[j + 1]);
        if (hd.htotalstat > helmetsarray[4].htotalstat)
            helmetsarray[4] = hd;
        for (int i = 0; i < 5 - 1; i++)
            for (int j = 0; j < 5 - i - 1; j++)
                if (helmetsarray[j].htotalstat < helmetsarray[j + 1].htotalstat)
					rpglib::swaphelmet(helmetsarray[j], helmetsarray[j + 1]);
    }
	//system("pause");
	//audio here
}

static int battle(character& c, weapon weaponsarray[], armor armorsarry[], helmet helmetsarray[], bool boss)
{
	system("CLS");
	cout << "When you see 'ATTACK #' hit the number then enter before times up" << endl;
	int rng = rand() % 3 + 6; //6-8 // ai hp rng
	int aihp = rng * aihpscaler();
	int numcheck = 10;
	int damage = 0;
	int totalmaxhp = c.mchp + c.ae.amchp + c.he.hmchp;

	bool keepatt = true;
	while (keepatt == true)
	{
		int aiatt = (((rand() % 12) + 16) * aiattscaler()); // rng att value
		int cntratt = (rand() % 3); // rng if cntratt. 2 in 3 chance
		int ct = ((rand() % 4) + 1) * 1000; // rng how long to wait for attack prompt
		int sucat = (rand() % 10); // rng number to enter

		if (c.dif == 2)
			aiatt = aiatt * 1.1; // +10% damage for ai in hardcore

		cout << "ai hp - " << aihp << "\nai attack - " << aiatt << "\n\n" << endl;

		numcheck = 10;
		system("pause");
		system("CLS");
		cout << "ATTACK: ";
		Sleep(ct);
		cout << sucat << endl;

		clock_t begin = clock();
		cin >> numcheck;
		clock_t end = clock();
		double atttime = double(end - begin) / CLOCKS_PER_SEC; // gets att time converted to seconds

		if (c.dif == 0) // half time for easy
			atttime = atttime / 2;

		system("CLS");

		cout << "time to strike: " << atttime << endl;

		if (cin.fail() == true) // non numerical input
		{
			cin.clear();
			cin.ignore(std::numeric_limits<int>::max(), '\n');
			cout << "You entered a character. " << numcheck << endl;
			numcheck = 10;
		}

		if (sucat == numcheck && atttime < 1) // hit
		{
			double dmg = 3 * ((double)c.catt + (c.we.wcatt + ((double)c.we.wcatt / 3) * (((double)c.lvl - 1) * .5))) * (1 - atttime);		//damage to scale based off time to hit. influence of weapon increase a little with each level to preserve usefullness. should hit for roughly what catt is on a mediocre hit.
			if (c.dif == 2)
				dmg = dmg * .9; // -10% damage for hardcore
			aihp = aihp - (int)dmg;
			cout << "HIT - " << (int)dmg << "\n" << endl;

			if (c.dif == 2) // crit needs .1 faster for hc. makes cntatt more frequant
			{
				if (atttime <= .5) // crit
				{
					cntratt = 2; // don't cntr att on crit
					mciSendString(L"seek critsfx to start", NULL, 0, NULL);
					mciSendString(L"play critsfx", NULL, 0, NULL);
				}
			}
			else
			{
				if (atttime <= .6)
				{
					cntratt = 2;
					mciSendString(L"seek critsfx to start", NULL, 0, NULL);
					mciSendString(L"play critsfx", NULL, 0, NULL);
				}
			}

			if (cntratt != 2 && aihp > 0)
			{
				c.chp = c.chp - aiatt / (cntratt + 2); // 50/50 between half and third att damage
				cout << "OUCH - Counter Attacked - " << (aiatt / (cntratt + 2)) << "\n" << endl;
			}
		}
		else
		{ // miss
			c.chp = c.chp - aiatt;
			cout << "OUCH" << endl;
			if (sucat != numcheck) // display entered number if it was wrong. otherwise will still have tts up
				cout << "You entered " << numcheck << " instead of " << sucat << ".\n" << endl;
		}

		if (c.chp < 1) // character dead
		{
			cout << "hp - 0\nYour garbage and don't deserve a second chance looser. Your score was " << c.score << ".\n\n" << endl;
			cout << "ai hp - " << aihp << "\nai attack - " << aiatt << "\n\n" << endl;
			system("pause");
			exitdungeon();
			return 1;
		}
		if (aihp < 1) // enemy dead
		{
			mciSendString(L"seek vsfx to start", NULL, 0, NULL);
			mciSendString(L"play vsfx", NULL, 0, NULL);

			int xpgain = xpgainscaler();
			if (boss == true) // double boss xp. xp drop on normal dist curve
				xpgain = deadlibrary::rngnd(xpgainscaler(), xpgainscaler(), 1) * 2; //using rng as bounds makes it fairly random independant of precision already
			c.cxp = c.cxp + xpgain;
			cout << "victory! you gained " << xpgain << " xp.\nCurrent xp: " << c.cxp << "/" << (int)xpcostscaler() << endl;
			cout << "\nhp remaining - " << c.chp << "/" << totalmaxhp << "\n" << endl;
			c.score = c.score + xpgain;
			xpfound += xpgain;

			system("pause");
			system("CLS");
			break;
		}

		cout << "\n\nhp - " << c.chp << "/" << totalmaxhp << "\nattack - " << (int)((double)c.catt + (c.we.wcatt + ((double)c.we.wcatt / 3) * (((double)charlevel - 1) * .5))) << "\n\n" << endl;
		cout << "ai hp - " << aihp << "\nai attack - " << aiatt << "\n" << endl;
		cout << "1 - continue attacking\n2 - retreat" << endl;
		int temp3;
		cin >> temp3;
		if (temp3 == 2)
			keepatt = false;
		system("CLS");
	}
	return 4;
}

static void dungeonboss(character& c, weapon weaponsarray[], armor armorsarry[], helmet helmetsarray[])
{
	//not enough to make this worth using yet
}

static void exitdungeon()
{
	system("CLS");
	cout << "Loot collected\n" << endl;
	for (weapon item : wfound)
		if (item.wname != "")
			cout << item.wname << endl;
	for (armor item : afound)
		if (item.aname != "")
			cout << item.aname << endl;
	for (helmet item : hfound)
		if (item.hname != "")
			cout << item.hname << endl;
	cout << xpfound << " xp gained" << endl;
	system("pause");
	system("CLS");
}

static int bosscount()
{
	int count = 0;
	for (int y = 0; y < MAP_SIZE_X; y++) // find player start position
		for (int x = 0; x < MAP_SIZE_Y; x++)
			if (map[x][y] == 'B')
			{
				count++;
			}
	return count;
}

static int checkcollision(character& c, weapon weaponsarray[], armor armorsarry[], helmet helmetsarray[]) // 0 - alive, 1 - dead, 2 - exit
{
    if (map[charx][chary] == ITEM)
    {
        itemdrop(c, weaponsarray, armorsarry, helmetsarray);
        map[charx][chary] = '#';
        return 3;
    }
    if (map[charx][chary] == ENEMY)
    {
        int dead = battle(c, weaponsarray, armorsarry, helmetsarray, false); //return 1 if dead, 4 if alive
        map[charx][chary] = '#';
        return dead;
    }
    if (map[charx][chary] == BOSS)
    {
		dungeondiff += 3; // boss fight is same but enemy is harder by 2 levels
		int dead = battle(c, weaponsarray, armorsarry, helmetsarray, true);
		dungeondiff -= 3;
        //dungeonboss(c, weaponsarray, armorsarry, helmetsarray);
        map[charx][chary] = '#';

		if (bosscount() == 0) // spawn exit if all bosses are dead. extra precuation against getting stuck
			map[charx][chary - 1] = 'O';
        return dead;
    }
    if (map[charx][chary] == DRAYGA)
    {
	cout << "am drayga, am deadening" << endl;
    }
    if (map[charx][chary] == EXIT)
    {
        exitdungeon();
        map[charx][chary] = '#';
        return 2;
    }
	return 4;
}

int dungeon(int dungeondiff, int dungeontype, character &c, weapon weaponsarray[], armor armorsarry[], helmet helmetsarray[]) //return 0 if dungeon was exited, return 1 if player died
{
	charlevel = c.lvl;

	initfog();
	if (dungeontype == 0) // autogen off
	{
		int mapnumber = rand() % 3 + 1; //1-3 // rng pick map. first num is amount of maps available
		string mapfile = "maps/map" + to_string(mapnumber) + ".txt";
		string line; //read in map
		ifstream is(mapfile);
		for (int y = 0; y < MAP_SIZE_X; y++)
			for (int x = 0; x < MAP_SIZE_Y; x++)
				is >> map[x][y];
		findC();
	}
	else if (dungeontype == 2) // from TLE
	{
		string mapfile = "twilightsedge/dungeonmap.txt";
		string line; //read in map
		ifstream is(mapfile);
		for (int y = 0; y < MAP_SIZE_X; y++)
			for (int x = 0; x < MAP_SIZE_Y; x++)
				is >> map[x][y];
		findC();
	}
	else // autogen on
	{
		makemap();
	}
    
    updatefog();
    fog[charx][chary] = '.'; //only needed once so left out of repeated updatefog() func
    printmap();
    int input = 0;
	int collisionresult = 0;
    while (1) //main dungeon control loop. plays live independant of input
    {
        input = 0;
        switch (input = _getch()) // if an arrow key is down move in that direction, not if its into a wall
        {
        case KEY_UP:
            if (map[charx][chary - 1] != '@')
                chary--;
            break;
        case KEY_DOWN:
            if (map[charx][chary + 1] != '@')
                chary++;
            break;
        case KEY_LEFT:
            if (map[charx - 1][chary] != '@')
                charx--;
            break;
        case KEY_RIGHT:
            if (map[charx + 1][chary] != '@')
                charx++;
            break;
        default:
            break;
        }      
        updatefog();//update FOW then clear and reprint then do any collision actions
        printmap();
		if (map[charx][chary] != '#') //don't run all the collision checks if there is no collision
		{
			collisionresult = checkcollision(c, weaponsarray, armorsarry, helmetsarray); // 0 - alive, 1 - dead, 2 - exit, 3 - item, 4 - boss/enemy
			if (collisionresult == 1)
				return 1;
			if (collisionresult == 2)
				return 0;
			if (collisionresult == 4)
				printmap();
		}
    }
	return 0;
}