//     RPG V3.0.1				newcontent.alteredcontent/balancing.backendchanges/bugs

//BUGS
//keep testing inv managment
//test lvlup sfx
//test titles
//test new soundtrack

//EDITS
//merge catt and catlvlups into same variable.
//update pvp.cpp - sfx
//try fibonacci for late game scaling? < x^2 till x > 12

//FEATURES
//boss theme(?dissonent theme? tremelo bass, e,f#,g. melody around a#. dim5th tritone)
//colorize
//?story mode?
//?map/travel?
//have someone else put in multiple character slots

#pragma warning(disable: 4996)

#include "includes.h"

using namespace std;

struct character
{
	string cclass = "";
	string name = "";
	string nametitle = "";
	int chp = 0;
	int mchp = 0;
	double catt = 0;
	int attlvlups = 0;
	double regen = 0;
	int cxp = 0;
	int lvl = 0;
	int score = 0;
	int dif = 1;
	weapon we = {"", 0 }; //weapon equiped
	//weapon weaponsarray[5]; // doesn't work if these arrays are not here. idk why, they arn't used as far as i know. doesn't seem to matter what they're called
	armor ae = {"", 0, 0, 0 };
	//armor armorsarray[5];
	helmet he = {"", 0, 0, 0 };
	//helmet helmetsarray[5];
};
character c = {};


void viewhscores();
void updatehscores();
void lvlup();
void play();
void savegame();
void viewchar(int pac);
void loadchar();
int titlescreen();
void settings();

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
bool keepplaying = true;
bool lvlrdy = false;
int difsel = 1; // 1=normal, 2=hardcore, 5920000737=cheateasy
int FullScreen = 1; // 1=fs, 0=windowed
int mvolume = 1000; // = 0-10 * 100
int sfxvolume = 1000; // ^^^
int soundtracknum = 1;
//string charactername = "";



//character c; //active character
weapon weaponsarray[5]; //arrays for inv
armor armorsarray[5];
helmet helmetsarray[5];
//scalers
//((2.8 * pow(c.lvl,2)) + (20 * c.lvl) + starting value) // wolfram alpha
const double aiattscaler() { return (.02 * pow(c.lvl, 2)) + (.1 * (double)c.lvl) + 1; } //pow(1.17, c.lvl);
const double aihpscaler() { return (.35 * pow(c.lvl, 2)) + (2.5 * (double)c.lvl) + 9; } //(pow(1.23, c.lvl) * 9);
const double cattscaler() { return (c.catt + (.25 * (double)pow(c.attlvlups, 2) + 1.15 * (double)c.attlvlups + 1.5)); } //((c.catt * 1.2) + (c.we.wcatt * 1.1) + (sqrt(c.attlvlups) * c.attlvlups));
const double mchpscaler() { return ((double)c.mchp * 1.3 + (((double)c.ae.amchp + (double)c.he.hmchp) * 1.15)); }
const double xpgainscaler() { return ((rand() % 3) + (double)3) * (1 + (double)c.lvl / 2); }
const double xpcostscaler() { return ((10 * (double)pow(1.3, c.lvl)) - ((double)sqrt(c.lvl)) * 4); }
const double regenscaler() { return ((double)(c.regen + c.ae.aregen + c.he.hregen)) * (((double)c.mchp + (double)c.ae.amchp + (double)c.he.hmchp) / 3); } // * (1 - atttime) //global variables //global variables

weapon createweapon()
{
	string wname; // total name, part1, part2
	string wname1;
	string wname2;
	int wnamei = (rand() % 4); //title selections
	int wnamei2 = (rand() % 4);
	int wcatt = ((rand() % 10) + 1); // weapon power
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
			wname2 = "Death";
		else if (wnamei2 == 1)
			wname2 = "Hades";
		else if (wnamei2 == 2)
			wname2 = "Chaos";
		else
			wname2 = "Glory";
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
			wname1 = "staff of ";
		else if (wnamei == 1)
			wname1 = "dagger of ";
		else if (wnamei == 2)
			wname1 = "Hammer of ";
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
weapon wd = { "", 0 }; // for dropped items

armor createarmor()
{
	string aname; // works same as createweapon()
	string aname1;
	string aname2;
	int aname1i = (rand() % 2);
	int atotalstat = ((rand() % 10) + 1); // total power to be distributed between hp and regen
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
armor ad = { "", 0, 0, 0 };

helmet createhelmet()
{
	string hname; // worsk same as createweapon()
	string hname1;
	string hname2;
	int hname1i = (rand() % 2);
	int htotalstat = ((rand() % 10) + 1); // total power to be distributed between hp and regen
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
helmet hd = { "", 0, 0, 0 };

int titlescreen()
{
	bool resetmenu = true;

	if (FullScreen == 1) // if fs is set to yes do so
	{
		COORD ScreenBufferSize{ 1500, 300 };
		HANDLE ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		HWND ConsoleWindow = GetConsoleWindow();
		SetConsoleScreenBufferSize(ConsoleHandle, ScreenBufferSize);
		ShowWindow(ConsoleWindow, SW_MAXIMIZE);
	}

	while (resetmenu == true) // not tested
	{
		resetmenu = false;

		FILE* fptr = fopen("asciiart.txt", "r"); // this chunk does the title art
		char read_string[128];
		while (fgets(read_string, sizeof(read_string), fptr) != NULL)
			printf("%s", read_string);
		fclose(fptr);

		int choice;
		printf("\n\n\nPress 1 for pvp or press enter to continue\n");
		if (cin.get() == '1')
		{
			choice = pvp(); //launch pvp script. will end returning 1 if exits game from pvp()
			if (choice == 1)
				return 1;
			else if (choice == 0)
				resetmenu = true;
			else {}
		}
	}
	cin.seekg(0, ios::end); // no idea what this does but i'd rather not cuase permanate damage tryign to fiugre it out
	cin.clear();
	return 0;
}

void viewhscores()
{
	string line;
	ifstream sdata;
	sdata.open("hscores.txt"); //just prints lines from hscores

	cout << "High Scores" << endl;
	while (getline(sdata, line))
	{
		cout << line << endl;
	}
	sdata.close();
}

void updatehscores()
{
	if (c.dif == 2) // double score for hardcore
		c.score = c.score * 2;
	int scorearray[11] = { 0,0,0,0,0,0,0,0,0,0,0 }; //place holders
	string names[11] = { "","","","","","","","","","","" };

	string line;
	ifstream sdatai;
	sdatai.open("hscores.txt");

	int counter = 0;
	while (getline(sdatai, line)) //reads lines into place holders. data is stored in descending order and thus read in likewise
	{
		istringstream ss(line);
		ss >> scorearray[counter] >> names[counter];
		counter++;
	}
	sdatai.close();
	if (c.score >= scorearray[9]) // if list is full replace lowest entry with current games score
	{
		scorearray[10] = c.score;
		printf("Enter name for highscore table.\n");
		cin >> names[10];
		for (int j = 0; j < 10; j++) //bubble sort cuase its easy
		{
			for (int i = 0; i < 10 - j; i++)
			{
				if (scorearray[i] > scorearray[i + 1])
				{
					int temp = scorearray[i];
					scorearray[i] = scorearray[i + 1];
					scorearray[i + 1] = temp;

					string temp2 = names[i]; // keep naems array matching
					names[i] = names[i + 1];
					names[i + 1] = temp2;
				}
			}
		}
		ofstream sdatao;
		sdatao.open("hscores.txt"); //save sorted data
		sdatao << scorearray[10] << " " << names[10] << "\n" << scorearray[9] << " " << names[9] << "\n" << scorearray[8] << " " << names[8] << "\n" << scorearray[7] << " " << names[7] << "\n" << scorearray[6] << " " << names[6] << "\n" << scorearray[5] << " " << names[5] << "\n" << scorearray[4] << " " << names[4] << "\n" << scorearray[3] << " " << names[3] << "\n" << scorearray[2] << " " << names[2] << "\n" << scorearray[1] << " " << names[1];
		sdatao.close();
	}
}

void lvlup()
{
	for (int x = 0; x < 2; x++) // 2 SP per level
	{
		bool tryagain = false;
		do
		{
			int statup;
			if (x == 0) //upgrade twice prompt control
				cout << "Congratulations on reaching level " << c.lvl + 1 << ". You may upgrade twice.\n\n1 - Max HP\n2 - Regeneration\n3 - Attack" << endl;
			else
				cout << "And one more.\n\n1 - Max HP\n2 - Regeneration\n3 - Attack" << endl;

			cin >> statup;
			if (statup == 1) // max hp
			{
				double temp = c.mchp;
				c.mchp = mchpscaler();
				c.chp = c.chp + ((c.mchp - temp) / 2);
				tryagain = false;
			}
			else if (statup == 2) // regen
			{
				c.regen = c.regen + 1;
				tryagain = false;
			}
			else if (statup == 3) // att
			{
				c.attlvlups++;
				c.catt = cattscaler();
				tryagain = false;
			}
			else
				tryagain = true;
		} while (tryagain == true);
		system("CLS");
	}
	c.cxp = c.cxp - (int)xpcostscaler(); // reset xp and up lvl
	lvlrdy = false;
	c.lvl++;

	if (c.lvl == 5)
	{
		cout << "Your journey as a warrior has begun. Who will you be remembered as?" << endl;
		cin >> c.name;
		c.nametitle = c.name + " the distinguished";
	}
	if (c.lvl == 10)
		c.nametitle = c.name + " the prestigious";
	if (c.lvl == 15)
		c.nametitle = c.name + " the renowned";
	if (c.lvl == 20)
		c.nametitle = "the glorious " + c.name;
	if (c.lvl == 25)
		c.nametitle = c.name + " the legend";
	if (c.lvl == 30)
		c.nametitle = c.name + " the fabled";
		if (c.lvl == 35)
		c.nametitle = c.name + " the exalted";
		if (c.lvl == 40)
		c.nametitle = c.name + " the immortal";
		if (c.lvl == 50)
		c.nametitle == "Immortal High King " + c.name;
}

void play()
{
	cout << "When you see 'ATTACK #' hit the number then enter before times up" << endl;
	int rng = rand() % 5 + 6; //6-10 // ai diff
	int aihp = rng * aihpscaler();
	int numcheck = 10;
	int damage = 0;
	int dt = 4;

	bool boss = false;
	if (rng >= 9) // boss
	{
		boss = true;
		mciSendString(L"seek elitesfx to start", NULL, 0, NULL);
		mciSendString(L"play elitesfx", NULL, 0, NULL);
	}

	bool keepatt = true;
	while (keepatt == true)
	{
		int aiatt = (((rand() % 11) + 15) * aiattscaler()); // rng att value
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
			double dmg = 3.5 * ((double)c.catt + ((double)c.we.wcatt * (1 + ((double)c.lvl - 1) * .1))) * (1 - atttime);		//damage to scale based off time to hit. influence of weapon increase a little with each level to preserve usefullness
			if (c.dif == 2)
				dmg = dmg * .9; // -10% damage for hardcore
			aihp = aihp - (int)dmg;
			cout << "HIT - " << dmg << "\n" << endl;

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
			mciSendString(L"seek dsfx to start", NULL, 0, NULL);
			mciSendString(L"play dsfx", NULL, 0, NULL);

			cout << "hp - 0\nYour garbage and don't deserve a second chance looser. Your score was " << c.score << ".\n\n" << endl;
			cout << "ai hp - " << aihp << "\nai attack - " << aiatt << "\n\n" << endl;
			if (c.dif == 2) // if hardcore, creates empty char to overwrite save data
			{
				character q = { "", "", "", 200, 200, 10, 0, 3, 0, 1, 0, difsel};
				FILE* ofpb = fopen("chardata.bin", "wb");
				fwrite(&q, sizeof(character), 1, ofpb);
				fclose(ofpb);
				system("CLS");
			}
			mciSendString(L"pause fighttheme", NULL, 0, NULL);
			mciSendString(L"pause fighttheme2", NULL, 0, NULL);
			mciSendString(L"seek maintheme to start", NULL, 0, NULL);
			mciSendString(L"play maintheme repeat", NULL, 0, NULL);
			system("pause");
			system("CLS");
			if (c.dif != 0) // don't get score saved for easy
				updatehscores();
			viewhscores();
			system("pause");
			mciSendString(L"close maintheme", NULL, 0, NULL); // close game
			mciSendString(L"close em3", NULL, 0, NULL);
			mciSendString(L"close fighttheme", NULL, 0, NULL);
			mciSendString(L"close fighttheme2", NULL, 0, NULL);
			mciSendString(L"close critsfx", NULL, 0, NULL);
			mciSendString(L"close itemdsfx", NULL, 0, NULL);
			mciSendString(L"close elitesfx", NULL, 0, NULL);
			mciSendString(L"close vsfx", NULL, 0, NULL);
			mciSendString(L"close dsfx", NULL, 0, NULL);
			exit(0);
		}
		if (aihp < 1) // enemy dead
		{
			mciSendString(L"seek vsfx to start", NULL, 0, NULL);
			mciSendString(L"play vsfx", NULL, 0, NULL);

			int xpgain = xpgainscaler();
			c.cxp = c.cxp + xpgain;
			cout << "victory! you gained " << xpgain << " xp.\nCurrent xp: " << c.cxp << "/" << (int)xpcostscaler() << endl;
			cout << "\nhp remaining - " << c.chp << "\n" << endl;
			c.score = c.score + xpgain;
			if (c.cxp >= (int)xpcostscaler())
			{
				mciSendString(L"seek lvlupsfx to start", NULL, 0, NULL);
				mciSendString(L"play lvlupsfx", NULL, 0, NULL);
				cout << "LEVEL UP\n" << endl;
				lvlrdy = true;
			}
			if (boss == true)
			{
				int ld = (rand() % 2); // item drop chance
				if (ld == 1)
				{
					dt = (rand() % 3); // item drop type
					mciSendString(L"seek itemdsfx to start", NULL, 0, NULL);
					mciSendString(L"play itemdsfx", NULL, 0, NULL);
					if (dt == 0)
					{
						weapon wd = createweapon();
						cout << "You've found " << wd.wname << "!" << endl;
						for (int i = 0; i < 5 - 1; i++) // sort inv by descending power
							for (int j = 0; j < 5 - i - 1; j++)
								if (weaponsarray[j].wcatt < weaponsarray[j + 1].wcatt)
								{
									weapon temp = weaponsarray[j];
									weaponsarray[j] = weaponsarray[j + 1];
									weaponsarray[j + 1] = temp;
								}
						if (wd.wcatt > weaponsarray[4].wcatt) // if dropped item is better than worse inventory item
							weaponsarray[4] = wd;
						for (int i = 0; i < 5 - 1; i++) // sort inv by descending power again with new item
							for (int j = 0; j < 5 - i - 1; j++)
								if (weaponsarray[j].wcatt < weaponsarray[j + 1].wcatt)
								{
									weapon temp = weaponsarray[j];
									weaponsarray[j] = weaponsarray[j + 1];
									weaponsarray[j + 1] = temp;
								}
					}
					if (dt == 1) // same as weapon drop
					{
						armor ad = createarmor();
						cout << "You've found " << ad.aname << "!" << endl;
						for (int i = 0; i < 5 - 1; i++)
							for (int j = 0; j < 5 - i - 1; j++)
								if (armorsarray[j].atotalstat < armorsarray[j + 1].atotalstat)
								{
									armor temp = armorsarray[j];
									armorsarray[j] = armorsarray[j + 1];
									armorsarray[j + 1] = temp;
								}
						if (ad.atotalstat > armorsarray[4].atotalstat)
							armorsarray[4] = ad;
						for (int i = 0; i < 5 - 1; i++)
							for (int j = 0; j < 5 - i - 1; j++)
								if (armorsarray[j].atotalstat < armorsarray[j + 1].atotalstat)
								{
									armor temp = armorsarray[j];
									armorsarray[j] = armorsarray[j + 1];
									armorsarray[j + 1] = temp;
								}
					}
					if (dt == 2) // same as weapon drop
					{
						helmet hd = createhelmet();
						cout << "You've found " << hd.hname << "!" << endl;
						for (int i = 0; i < 5 - 1; i++)
							for (int j = 0; j < 5 - i - 1; j++)
								if (helmetsarray[j].htotalstat < helmetsarray[j + 1].htotalstat)
								{
									helmet temp = helmetsarray[j];
									helmetsarray[j] = helmetsarray[j + 1];
									helmetsarray[j + 1] = temp;
								}
						if (hd.htotalstat > helmetsarray[4].htotalstat)
							helmetsarray[4] = hd;
						for (int i = 0; i < 5 - 1; i++)
							for (int j = 0; j < 5 - i - 1; j++)
								if (helmetsarray[j].htotalstat < helmetsarray[j + 1].htotalstat)
								{
									helmet temp = helmetsarray[j];
									helmetsarray[j] = helmetsarray[j + 1];
									helmetsarray[j + 1] = temp;
								}
					}
				}
			}
			system("pause");
			system("CLS");


			int sucheal = (rand() % 10); // same as attack but heals instead
			cout << "Time to get some hp back, make it count.\n" << endl;
			system("pause");
			cout << "HEAL: ";
			Sleep(ct);
			cout << sucheal << endl;
			clock_t begin = clock();
			cin >> numcheck;
			clock_t end = clock();
			double atttime = double(end - begin) / CLOCKS_PER_SEC;
			system("CLS");
			cout << "time for regen: " << atttime << endl;

			if (sucheal == numcheck && atttime < 1)
			{
				int temp = c.chp;
				c.chp = c.chp + (regenscaler() * (1 - atttime));
				temp = c.chp - temp;
				cout << temp << " hp gained." << endl;
				if (c.chp > (c.mchp + c.ae.amchp + c.he.hmchp))
					c.chp = (c.mchp + c.ae.amchp + c.he.hmchp);
			}
			else
			{
				cout << "Your actually throwing, that was too important to choke this hard." << endl;
			}
			system("pause");
			system("CLS");

			break;
		}

		cout << "ai hp - " << aihp << "\nai attack - " << aiatt << "\n" << endl;
		cout << "\nhp - " << c.chp << "/" << c.mchp << "\nattack - " << c.catt + c.we.wcatt << "\n\n" << endl;
		cout << "1 - continue attacking\n2 - retreat" << endl;
		int temp3;
		cin >> temp3;
		if (temp3 == 2)
			keepatt = false;
		system("CLS");
	}
	
}

void viewchar(int pac)
{
	if (c.dif == 2) // display in red for HC character
		SetConsoleTextAttribute(hConsole, 4);
	if (c.name == "") //show name with title if applicable
		cout << "Class - " << c.cclass << endl;
	else
		cout << c.name << endl;
	cout << "HP - " << c.chp << "/" << c.mchp + c.ae.amchp + c.he.hmchp << endl;
	cout << "Regeneration - " << c.regen + c.ae.aregen + c.he.hregen << endl;
	cout << "attack - " << (int)((double)c.catt + ((double)c.we.wcatt * (1 + ((double)c.lvl - 1) * .1))) << " : weapon - " << (int)((double)c.we.wcatt * (1 + ((double)c.lvl - 1) * .1)) << endl;
	cout << "Expierience - " << c.cxp << "/" << (int)xpcostscaler() << endl;
	cout << "Level - " << (int)c.lvl << "\n\n" << endl;
	cout << "\nCurrent Weapon: " << c.we.wname << "\nCurrent Armor: " << c.ae.aname << "\nCurrent Helmet: " << c.he.hname << endl;
	if (c.dif == 2) // put color back normal
		SetConsoleTextAttribute(hConsole, 15);

	if (pac == 1) // enables inventory managment option. not sure when viewchar is called without it
	{
		bool skip = false;
		bool ri = false;
		char mc;
		int mc2;
		int mc3;
		char mc4;
		int mc5;
		cout << "1 - Manage Inventory\n2 - Exit" << endl;
		cin >> mc3;
		if (mc3 == 1)
		{
			weapon emptyweapon = { "", 0 };
			weapon emptyweapon1 = { "", 0 };
			armor emptyarmor = { "", 0, 0, 0, };
			helmet emptyhelmet = { "", 0, 0, 0 };
			do
			{
				ri = false;
				system("CLS");
				cout << "\nCurrent Weapon: " << c.we.wname << " - " << c.we.wcatt << " att" << "\nCurrent Armor: " << c.ae.aname << " - " << c.ae.amchp << " hp - " << c.ae.aregen << "regen" << "\nCurrent Helmet: " << c.he.hname << " - " << c.he.hmchp << " hp - " << c.he.hregen << " regen" << endl;


				cout << "\nA - Weapons in inventory." << endl;
				for (int i = 0; i < 5; i++)
				{
					if (weaponsarray[i].wname == "" || weaponsarray[i].wname == " ") //empty naems should be blank but just in case its a space it checks that too
						cout << i + 1 << " - " << "empty" << endl;
					else
						cout << i + 1 << " - " << weaponsarray[i].wname << " - " << weaponsarray[i].wcatt << " att" << endl;
				}
				cout << "\nB - Armors in inventory." << endl;
				for (int i = 0; i < 5; i++)
				{
					if (armorsarray[i].aname == "" || armorsarray[i].aname == " ")
						cout << i + 1 << " - " << "empty" << endl;
					else
						cout << i + 1 << " - " << armorsarray[i].aname << " - " << armorsarray[i].amchp << " hp" << " - " << armorsarray[i].aregen << " regen" << endl;
				}
				cout << "\nC - Helmets in inventory." << endl;
				for (int i = 0; i < 5; i++)
				{
					if (helmetsarray[i].hname == "" || helmetsarray[i].hname == " ")
						cout << i + 1 << " - " << "empty" << endl;
					else
						cout << i + 1 << " - " << helmetsarray[i].hname << " - " << helmetsarray[i].hmchp << " hp" << " - " << helmetsarray[i].hregen << " regen" << endl;
				}

				cout << "\nTo switch items enter the letter of the item type you would like to replace, or D to delete/unequip. Enter 0 at any time to exit." << endl;
				cin >> mc;
				if (mc == 'a' || mc == 'A')
				{
					cout << "Enter the number of the weapon you would like to equip." << endl;
					cin >> mc2;
					if (mc2 == 1)
						deadlibrary::dlclass::swapweapon(c.we, weaponsarray[0]);
					else if (mc2 == 2)
						deadlibrary::dlclass::swapweapon(c.we, weaponsarray[1]);
					else if (mc2 == 3)
						deadlibrary::dlclass::swapweapon(c.we, weaponsarray[2]);
					else if (mc2 == 4)
						deadlibrary::dlclass::swapweapon(c.we, weaponsarray[3]);
					else if (mc2 == 5)
						deadlibrary::dlclass::swapweapon(c.we, weaponsarray[4]);
					//else if (mc2 == 6) ////////// forr debugging incase it crashes again
					//{
						//emptyweapon = c.we;
						//emptyweapon1 = weaponsarray[0];
						//cout << emptyweapon.wname << emptyweapon1.wname;
						//deadlibrary::dlclass::swapweapon(emptyweapon, emptyweapon1);
						//cout << emptyweapon.wname << emptyweapon1.wname;
						//system("pause");
					//}
					else
						skip = true;
				}
				else if (mc == 'b' || mc == 'B')
				{
					cout << "Enter the number of the armor you would like to equip." << endl;
					cin >> mc2;
					if (mc2 == 1)
						deadlibrary::dlclass::swaparmor(c.ae, armorsarray[0]);
					else if (mc2 == 2)
						deadlibrary::dlclass::swaparmor(c.ae, armorsarray[1]);
					else if (mc2 == 3)
						deadlibrary::dlclass::swaparmor(c.ae, armorsarray[2]);
					else if (mc2 == 4)
						deadlibrary::dlclass::swaparmor(c.ae, armorsarray[3]);
					else if (mc2 == 5)
						deadlibrary::dlclass::swaparmor(c.ae, armorsarray[4]);
					else
						skip = true;
				}
				else if (mc == 'c' || mc == 'C')
				{
					cout << "Enter the number of the helmet you would like to equip." << endl;
					cin >> mc2;
					if (mc2 == 1)
						deadlibrary::dlclass::swaphelmet(c.he, helmetsarray[0]);
					else if (mc2 == 2)
						deadlibrary::dlclass::swaphelmet(c.he, helmetsarray[1]);
					else if (mc2 == 3)
						deadlibrary::dlclass::swaphelmet(c.he, helmetsarray[2]);
					else if (mc2 == 4)
						deadlibrary::dlclass::swaphelmet(c.he, helmetsarray[3]);
					else if (mc2 == 5)
						deadlibrary::dlclass::swaphelmet(c.he, helmetsarray[4]);
					else
						skip = true;
				}
				else if (mc == 'd' || mc == 'D')
				{
					ri = true;
					cout << "enter the letter of the item type you would like to delete/unequip." << endl;
					cin >> mc4;
					if (mc4 == 'a' || mc == 'A')
					{
						cout << "Enter the number of the weapon you would like to delete or enter 6 to unequip your current weapon." << endl;
						cin >> mc5;
						if (mc5 == 1)
							weaponsarray[0] = emptyweapon;
						else if (mc5 == 2)
							weaponsarray[1] = emptyweapon;
						else if (mc5 == 3)
							weaponsarray[2] = emptyweapon;
						else if (mc5 == 4)
							weaponsarray[3] = emptyweapon;
						else if (mc5 == 5)
							weaponsarray[4] = emptyweapon;
						else if (mc5 == 6)
						{
							weaponsarray[4] = c.we; // already sorted so worse item is in last slot
							c.we = emptyweapon;
							for (int i = 0; i < 5 - 1; i++) // sort again as theres a new item in inv
								for (int j = 0; j < 5 - i - 1; j++)
									if (weaponsarray[j].wcatt < weaponsarray[j + 1].wcatt)
										deadlibrary::dlclass::swapweapon(weaponsarray[j], weaponsarray[j + 1]);
						}
					}
					else if (mc4 == 'b' || mc == 'B')
					{
						cout << "Enter the number of the armor you would like to delete or enter 6 to unequip your current armor." << endl;
						cin >> mc5;
						if (mc5 == 1)
							armorsarray[0] = emptyarmor;
						else if (mc5 == 2)
							armorsarray[1] = emptyarmor;
						else if (mc5 == 3)
							armorsarray[2] = emptyarmor;
						else if (mc5 == 4)
							armorsarray[3] = emptyarmor;
						else if (mc5 == 5)
							armorsarray[4] = emptyarmor;
						else if (mc5 == 6)
						{
							armorsarray[4] = c.ae;
							c.ae = emptyarmor;
							for (int i = 0; i < 5 - 1; i++)
								for (int j = 0; j < 5 - i - 1; j++)
									if (armorsarray[j].atotalstat < armorsarray[j + 1].atotalstat)
										deadlibrary::dlclass::swaparmor(armorsarray[j], armorsarray[j + 1]);
						}
					}
					else if (mc4 == 'c' || mc == 'C')
					{
						cout << "Enter the number of the helmet you would like to delete or enter 6 to unequip your current helmet." << endl;
						cin >> mc5;
						if (mc5 == 1)
							helmetsarray[0] = emptyhelmet;
						else if (mc5 == 2)
							helmetsarray[1] = emptyhelmet;
						else if (mc5 == 3)
							helmetsarray[2] = emptyhelmet;
						else if (mc5 == 4)
							helmetsarray[3] = emptyhelmet;
						else if (mc5 == 5)
							helmetsarray[4] = emptyhelmet;
						else if (mc5 == 6)
						{
							helmetsarray[4] = c.he;
							c.he = emptyhelmet;
							for (int i = 0; i < 5 - 1; i++)
								for (int j = 0; j < 5 - i - 1; j++)
									if (helmetsarray[j].htotalstat < helmetsarray[j + 1].htotalstat)
										deadlibrary::dlclass::swaphelmet(helmetsarray[j], helmetsarray[j + 1]);
						}
					}
					else
						skip = true;
				}
			} while (ri == true);
		}
		else
		{
			skip = true;
		}

		if (skip == false)
		{
			system("CLS");
			if (c.name == "")
				cout << "Class - " << c.cclass << endl;
			else
				cout << c.name << endl;
			cout << "HP - " << c.chp << "/" << c.mchp + c.ae.amchp + c.he.hmchp << endl;
			cout << "Regeneration - " << c.regen + c.ae.aregen + c.he.hregen << endl;
			cout << "attack - " << (int)((double)c.catt + ((double)c.we.wcatt * (1 + ((double)c.lvl - 1) * .1))) << " : weapon - " << (int)((double)c.we.wcatt * (1 + ((double)c.lvl - 1) * .1)) << endl;
			cout << "Expierience - " << c.cxp << "/" << (int)xpcostscaler() << endl;
			cout << "Level - " << c.lvl << "\n\n" << endl;
			cout << "\nCurrent Weapon: " << c.we.wname << "\nCurrent Armor: " << c.ae.aname << "\nCurrent Helmet: " << c.he.hname << endl;
			system("pause");
		}
	}
	system("CLS");
}

void savegame()
{
	ofstream out1("char/weapons.txt"); // saves an inventory item type
	out1 << c.we.wcatt << " " << c.we.wname << "\n";
	for (int i = 0; i < 5; i++)
		out1 << weaponsarray[i].wcatt << " " << weaponsarray[i].wname << "\n";

	ofstream out2("char/armors.txt");
	out2 << c.ae.atotalstat << " " << c.ae.aregen << " " << c.ae.amchp << " " << c.ae.aname << "\n";
	for (int i = 0; i < 5; i++)
		out2 << armorsarray[i].atotalstat << " " << armorsarray[i].aregen << " " << armorsarray[i].amchp << " " << armorsarray[i].aname << "\n";

	ofstream out3("char/helmets.txt");
	out3 << c.he.htotalstat << " " << c.he.hregen << " " << c.he.hmchp << " " << c.he.hname << "\n";
	for (int i = 0; i < 5; i++)
		out3 << helmetsarray[i].htotalstat << " " << helmetsarray[i].hregen << " " << helmetsarray[i].hmchp << " " << helmetsarray[i].hname << "\n";

	out1.close();
	out2.close();
	out3.close();
	/*
	FILE* ofpb = fopen("char/chardata.bin", "wb"); //save char
	fwrite(&c, sizeof(character) + sizeof(weapon) + sizeof(armor) + sizeof(helmet), 1, ofpb);
	fclose(ofpb);
	*/

	ofstream out4("char/chardata.txt"); //cclass is one word so it can be read in like the other vars
	out4 << c.name << "\n" << c.nametitle << "\n" << c.cclass << " " << c.chp << " " << c.mchp << " " << c.catt << " " << c.attlvlups << " " << c.regen << " " << c.cxp << " " << c.lvl << " " << c.score << " " << c.dif << "\n";
	out4.close();
	system("CLS");
}

void loadchar()
{
	

	/*
	FILE* ifpb = fopen("char/chardata.bin", "rb"); //load char
	fread(&c, sizeof(character) + sizeof(weapon) + sizeof(armor) + sizeof(helmet), 1, ifpb);
	fclose(ifpb);
	*/

	//string t;

	ifstream infile;
	infile.open("char/chardata.txt");
	string textline;
	getline(infile, textline, '\n');
	istringstream name(textline);
	name >> c.name;
	getline(infile, textline, '\n');
	istringstream nametitle(textline);
	nametitle >> c.nametitle;
	getline(infile, textline);
	istringstream data(textline);
	data >> c.cclass >> c.chp >> c.mchp >> c.catt >> c.attlvlups >> c.regen >> c.cxp >> c.lvl >> c.score >> c.dif;
	
	//system("pause");

	c.we = {}; //idk if i need this
	c.we.wname = "";
	c.we.wcatt = 0;
	c.ae = {};
	c.ae.aname = "";
	c.ae.amchp = 0;
	c.ae.aregen = 0;
	c.ae.atotalstat = 0;
	c.he = {};
	c.he.hname = "";
	c.he.hmchp = 0;
	c.he.hregen = 0;
	c.he.htotalstat = 0;
	for (int i = 0; i < 5; i++)
	{
		weaponsarray[i] = { "", 0 };
		armorsarray[i] = { "", 0, 0, 0 };
		helmetsarray[i] = { "", 0, 0, 0 };
	}



	//c.we = { };
	string w1, w2, w3; //loads an inventory item type
	string line1;
	ifstream sdata1;
	sdata1.open("char/weapons.txt");
	int counter1 = 0;
	getline(sdata1, line1);
	istringstream iss1(line1);
	iss1 >> c.we.wcatt >> w1 >> w2 >> w3;
	//cout << c.we.wcatt << "\n";////////////////////////////////////////////////
	//system("pause");
	//cout << w1 << "\n";////////////////////////////////////////////////
	//system("pause");
	//cout << w2 << "\n";////////////////////////////////////////////////
	//system("pause");
	//cout << w3 << "\n";////////////////////////////////////////////////
	//system("pause");
	//string tempdebug = w1 + " " + w2 + " " + w3;
	//cout << tempdebug << "\n";////////////////////////////////////////////////
	//system("pause");
	//cout << c.we.wname << "\n";////////////////////////////////////////////////
	//system("pause");
	//c.we.wname = w1 + " " + w2 + " " + w3;
	//cout << c.we.wname << "\n";////////////////////////////////////////////////
	//system("pause");
	while (getline(sdata1, line1)) 
	{
		istringstream ssi(line1);
		ssi >> weaponsarray[counter1].wcatt >> w1 >> w2 >> w3;
		weaponsarray[counter1].wname = w1 + " " + w2 + " " + w3;
		counter1++;
		cout << weaponsarray[counter1].wname << "\n";////////////////////////////////////////////////
		system("pause");
	}
	sdata1.close();

	string a1, a2, a3;
	string line2;
	ifstream sdata2;
	sdata2.open("char/armors.txt");
	int counter2 = 0;
	getline(sdata2, line2);
	istringstream iss2(line2);
	iss2 >> c.ae.atotalstat >> c.ae.aregen >> c.ae.amchp >> a1 >> a2 >> a3;
	c.ae.aname = a1 + " " + a2 + " " + a3;
	while (getline(sdata2, line2))
	{
		istringstream ss2(line2);
		ss2 >> armorsarray[counter2].atotalstat >> armorsarray[counter2].aregen >> armorsarray[counter2].amchp >> a1 >> a2 >> a3;
		armorsarray[counter2].aname = a1 + " " + a2 + " " + a3;
		counter2++;
	}
	sdata2.close();

	string h1, h2, h3;
	string line3;
	ifstream sdata3;
	sdata3.open("char/helmets.txt");
	int counter3 = 0;
	getline(sdata3, line3);
	stringstream iss3(line3);
	iss3 >> c.he.htotalstat >> c.he.hregen >> c.he.hmchp >> h1 >> h2 >> h3;
	c.he.hname = h1 + " " + h2 + " " + h3;
	while (getline(sdata3, line3))
	{
		istringstream ss3(line3);
		ss3 >> helmetsarray[counter3].htotalstat >> helmetsarray[counter3].hregen >> helmetsarray[counter3].hmchp >> h1 >> h2 >> h3;
		helmetsarray[counter3].hname = h1 + " " + h2 + " " + h3;
		counter3++;
	}
	sdata3.close();

	system("CLS");
}

void volumeupdate()
{
	string vol;
	vol = "setaudio em3 volume to ";
	vol.append(to_string(mvolume));
	mciSendStringA(vol.c_str(), NULL, 0, 0);
	vol = "setaudio fighttheme volume to ";
	vol.append(to_string(mvolume));
	mciSendStringA(vol.c_str(), NULL, 0, 0);
	vol = "setaudio fighttheme2 volume to ";
	vol.append(to_string(mvolume));
	mciSendStringA(vol.c_str(), NULL, 0, 0);
	vol = "setaudio maintheme volume to ";
	vol.append(to_string(mvolume));
	mciSendStringA(vol.c_str(), NULL, 0, 0);
	vol = "setaudio titletheme volume to ";
	vol.append(to_string(mvolume));
	mciSendStringA(vol.c_str(), NULL, 0, 0);
	vol = "setaudio critsfx volume to ";
	vol.append(to_string(sfxvolume));
	mciSendStringA(vol.c_str(), NULL, 0, NULL);
	vol = "setaudio itemdsfx volume to ";
	vol.append(to_string(sfxvolume));
	mciSendStringA(vol.c_str(), NULL, 0, NULL);
	vol = "setaudio elitesfx volume to ";
	vol.append(to_string(sfxvolume));
	mciSendStringA(vol.c_str(), NULL, 0, NULL);
	vol = "setaudio vssfx volume to ";
	vol.append(to_string(sfxvolume));
	mciSendStringA(vol.c_str(), NULL, 0, NULL);
	vol = "setaudio dssfx volume to ";
	vol.append(to_string(sfxvolume));
	mciSendStringA(vol.c_str(), NULL, 0, NULL);
}

void settings()
{
	mciSendString(L"open audio/elitesfx.mp3 type mpegvideo alias elitesfx", NULL, 0, NULL); // to hear sfx whe nselecting
	int cs = 0;
	int mc = 0;
	bool vi = true;
	bool loop;
	do
	{
		system("CLS");
		cout << "Settings" << endl;
		cout << "1 - New Character Difficulty\n2 - Toggle Full Screen launch\n3 - Music Volume\n4 - Sound Effects Volume\n5 - Change soundtrack (relaunch game).\n6 - exit" << endl;

		vi = false;
		cin >> cs;
		switch (cs)
		{
		case 1: int diff;
			loop = false;
			do {
				cout << "Select difficulty for new Characters.\n1 - normal\n2 - hardcore." << endl;
				cin >> diff;
				if (diff == 2 || diff == 1)
					difsel = diff;
				else if (diff == 5920000737)
					difsel = 0;
				else
				{
					system("CLS");
					loop = true;
					cout << "invalid input" << endl;
				}
			} while (loop == true);
			break;
		case 2:
			if (FullScreen == 1)
			{
				FullScreen = 0;
				COORD ScreenBufferSize{ 120, 300 };
				HANDLE ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
				HWND ConsoleWindow = GetConsoleWindow();
				SetConsoleScreenBufferSize(ConsoleHandle, ScreenBufferSize);
				ShowWindow(ConsoleWindow, SW_NORMAL);
			}
			else
			{
				FullScreen = 1;
				COORD ScreenBufferSize{ 1500, 300 };
				HANDLE ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
				HWND ConsoleWindow = GetConsoleWindow();
				SetConsoleScreenBufferSize(ConsoleHandle, ScreenBufferSize);
				ShowWindow(ConsoleWindow, SW_MAXIMIZE);
			}
			break;
		case 3: cout << "Enter a music volume level 0 - 10" << endl;
			cin >> mvolume;
			mvolume *= 100;
			volumeupdate();
			break;
		case 4: mciSendString(L"seek elitesfx to start", NULL, 0, NULL);
			mciSendStringA("play elitesfx", NULL, 0, NULL);
			cout << "Enter a sound effects volume level 0 - 10" << endl;
			cin >> sfxvolume;
			sfxvolume *= 100;
			volumeupdate();
			break;
		case 5: cout << "Judah - 1\nAlina - 2" << endl;
			cin >> soundtracknum;
			break;
		case 6: vi = true;
			keepplaying = false;
			break;
		default: vi = false;
			break;
		}
	} while (vi != true);
	system("CLS");

	FILE* ofp = fopen("settings.txt", "w"); //update settigns file
	fprintf(ofp, "FullScreen %d\nStart Difficulty %d\nMusic Volume %d\nEffects Volume %d\nSoundtrack %d", FullScreen, difsel, mvolume, sfxvolume, soundtracknum);
	fclose(ofp);
	mciSendString(L"close elitesfx", NULL, 0, NULL);
}

int main()
{
	int dt; ///////////////////////////////////////////////item create for 6
	//SetConsoleTextAttribute(hConsole, 15);

	string st;
	FILE* ifp = fopen("settings.txt", "r"); //read in settigns
	fscanf(ifp, "FullScreen %d\n", &FullScreen);
	fscanf(ifp, "Start Difficulty %d\n", &difsel);
	fscanf(ifp, "Music Volume %d\n", &mvolume);
	fscanf(ifp, "Effects Volume %d\n", &sfxvolume);
	fscanf(ifp, "Soundtrack %d", &soundtracknum);
	if (soundtracknum == 2)
		st = "st2";
	else
		st = "st1";
	fclose(ifp);

	string em3 = "open audio/" + st + "/titletheme.mp3 mpegvideo alias em3"; //creating all needed audio players
	string fighttheme = "open audio/" + st + "/fighttheme.mp3 type mpegvideo alias fighttheme";
	string fighttheme2 = "open audio/" + st + "/fighttheme2.mp3 type mpegvideo alias fight2theme";
	string maintheme = "open audio/" + st + "/maintheme.mp3 type mpegvideo alias maintheme";
	string titletheme = "open audio/" + st + "/titletheme.mp3 type mpegvideo alias titletheme";
	string practicetheme = "open audio/" + st + "/practicetheme.mp3 type mpegvideo alias practicetheme";
	mciSendStringA(em3.c_str(), NULL, 0, NULL);
	mciSendStringA(fighttheme2.c_str(), NULL, 0, NULL);
	mciSendStringA(fighttheme2.c_str(), NULL, 0, NULL);
	mciSendStringA(maintheme.c_str(), NULL, 0, NULL);
	mciSendStringA(titletheme.c_str(), NULL, 0, NULL);
	mciSendStringA(practicetheme.c_str(), NULL, 0, NULL);
	mciSendString(L"open audio/sfx/crit.mp3 type mpegvideo alias critsfx", NULL, 0, NULL);
	mciSendString(L"open audio/sfx/itemd.mp3 type mpegvideo alias itemdsfx", NULL, 0, NULL);
	mciSendString(L"open audio/sfx/elite.mp3 type mpegvideo alias elitesfx", NULL, 0, NULL);
	mciSendString(L"open audio/sfx/v.mp3 type mpegvideo alias vsfx", NULL, 0, NULL);
	mciSendString(L"open audio/sfx/d.mp3 type mpegvideo alias dsfx", NULL, 0, NULL);
	mciSendString(L"open audio/sfx/lvlup.mp3 type mpegvideo alias lvlupsfx", NULL, 0, NULL);

	int mmc = 0;
	int temp2 = 0;
	bool resetmenu = true;
	srand((unsigned)time(0));
	int ts;
	
	volumeupdate();

	mciSendString(L"play titletheme repeat", NULL, 0, NULL);
	int pvpfullexit = titlescreen(); // returns 1 if pvp() wants to close app
	mciSendString(L"pause titletheme", NULL, 0, NULL);
	mciSendString(L"close titletheme", NULL, 0, NULL);
	system("CLS");

	if (pvpfullexit == 1) // app closed form pvp()
	{
		mciSendString(L"close maintheme", NULL, 0, NULL);
		mciSendString(L"close em3", NULL, 0, NULL);
		mciSendString(L"close fighttheme", NULL, 0, NULL);
		mciSendString(L"close fighttheme2", NULL, 0, NULL);
		return 0;
	}

	do
	{
		mciSendString(L"seek maintheme to start", NULL, 0, NULL);
		mciSendString(L"play maintheme repeat", NULL, 0, NULL);
		resetmenu = true;
		cout << "1 - Start a New Game\n2 - Load Game\n3 - Gameplay Mechanics\n4 - Combat Practice\n5 - High Scores\n6 - settings\n7 - Exit Game" << endl;
		cin >> temp2;
		system("CLS");

		if (temp2 == 1)
		{
			cout << "What type of character you would like to make?\n1 - Orc - high health but low damage. Makes for a forgving combat experience.\n2 - Human - medium health and medium damage. A good balance in required combat ability. \n3 - Elf - low health but high damage. Requires the ability to consistently win rounds." << endl;
			int temp;
			cin >> temp;
			system("CLS");
			switch (temp)
			{
			case 1: c.cclass = "Orc";
				  c.name = "";
				  c.nametitle = "";
				  c.chp = 200;
				  c.mchp = 200;
				  c.catt = 10;
				  c.attlvlups = 0;
				  c.regen = 3;
				  c.cxp = 0;
				  c.lvl = 1;
				  c.score = 0;
				  c.dif = difsel;
				  break;
			case 2: c.cclass = "Human";
				  c.name = "";
				  c.nametitle = "";
				  c.chp = 100;
				  c.mchp = 100;
				  c.catt = 20;
				  c.attlvlups = 0;
				  c.regen = 3;
				  c.cxp = 0;
				  c.lvl = 1;
				  c.score = 0;
				  c.dif = difsel;
				  break;
			case 3: c.cclass = "Elf";
				  c.name = "";
				  c.nametitle = "";
				  c.chp = 50;
				  c.mchp = 50;
				  c.catt = 40;
				  c.attlvlups = 0;
				  c.regen = 3;
				  c.cxp = 0;
				  c.lvl = 1;
				  c.score = 0;
				  c.dif = difsel;
				  break;
			default:
				break;
			}
			c.we = {};
			c.we.wname = "";
			c.we.wcatt = 0;
			c.ae = {};
			c.ae.aname = "";
			c.ae.amchp = 0;
			c.ae.aregen = 0;
			c.ae.atotalstat = 0;
			c.he = {};
			c.he.hname = "";
			c.he.hmchp = 0;
			c.he.hregen = 0;
			c.he.htotalstat = 0;
			for (int i = 0; i < 5; i++)
			{
				weaponsarray[i] = { "", 0 };
				armorsarray[i] = { "", 0, 0, 0 };
				helmetsarray[i] = { "", 0, 0, 0 };
			}
			system("pause");
		}
		else if (temp2 == 2)
		{
			loadchar();
		}
		else if (temp2 == 3)
		{
			mciSendString(L"pause maintheme", NULL, 0, NULL);
			mciSendString(L"seek em3 to start", NULL, 0, NULL);
			mciSendString(L"play em3 repeat", NULL, 0, NULL);
			cout << "To win combat, hit your opponent before they hit you.\nAttacking is done by entering the number shown next to attack: then hitting the enter key.\nAttacks that take under 1 second will deal damage increasing with how fast you were.\nAttacks under .6 seconds will be critical hits, preventing counter attacks.\nAttacks that miss or hit too late will result in your opponent winning the round and doing full damage to you.\n\nAi hp and damage decided through rng that scales with player level. Damage changes each round.\n\nXP gain and level up requirements also scaled with player level.\n\nFor non-critical hits, there is an even chance of counter attack damage between 0, half, and a third of enemy's attack.\n\nAfter defeating an enemy, using the same mechanics as attacking, you are able to regen hp by an amount scaling\nwith your max hp as well your regeneration stat.\n\ndon't miss your chance to level up, if you earn another one with the previous still pending you'll loose it.\n\nHardcore difficulty will erase save data of a character that dies but scroe will be doubled.\nHardcore also lowers player damage and increases enemy health, both by 10%.\nCritical hits will also require a time of under .5 seconds.\n" << endl;
			system("pause");
			system("CLS");
			keepplaying = false;
			mciSendString(L"pause em3", NULL, 0, NULL);
		}
		else if (temp2 == 4)
		{ // basically a copy of play()
			mciSendString(L"pause maintheme", NULL, 0, NULL);
			mciSendString(L"seek practicetheme to start", NULL, 0, NULL);
			mciSendString(L"play practicetheme repeat", NULL, 0, NULL);
			bool cont = true;
			int numcheck = 11;
			system("CLS");
			cout << "Enter 10 for an attack to quit." << endl;

			while (cont == true)
			{
				int ct = ((rand() % 5) + 1) * 1000;
				int sucat = (rand() % 10);
				int cntratt = (rand() % 2);

				cout << "ATTACK: ";
				Sleep(ct);
				cout << sucat << endl;
				numcheck = 11;

				clock_t begin = clock();

				cin >> numcheck;

				clock_t end = clock();

				double atttime = double(end - begin) / CLOCKS_PER_SEC;

				system("CLS");

				cout << "time to strike: " << atttime << endl;

				if (cin.fail() == true)
				{
					cin.clear();
					cin.ignore(std::numeric_limits<int>::max(), '\n');
					cout << "you entered a character. " << numcheck << endl;
					numcheck = 11;
				}
				if (sucat == numcheck && atttime < 1)
				{
					cout << "HIT" << "\n" << endl;
					if (atttime < .6)
						cntratt = 1;
					if (cntratt == 0)
						cout << "OUCH - Counter Attacked\n" << endl;
				}
				else
				{
					cout << "OUCH\n" << endl;
					if (sucat != numcheck)
						cout << "You entered " << numcheck << " instead of " << sucat << "." << endl;
				}
				if (numcheck == 10)
				{
					cont = false;
					break;
				}
				system("pause");
				system("CLS");
			}
			keepplaying = false;
			system("CLS");
			mciSendString(L"pause practicetheme", NULL, 0, NULL);
			mciSendString(L"seek maintheme to start", NULL, 0, NULL);
			mciSendString(L"play maintheme repeat", NULL, 0, NULL);
		}
		else if (temp2 == 5)
		{
			mciSendString(L"pause maintheme", NULL, 0, NULL);
			mciSendString(L"seek em3 to start", NULL, 0, NULL);
			mciSendString(L"play em3 repeat", NULL, 0, NULL);
			system("CLS");
			viewhscores();
			system("pause");
			system("CLS");
			keepplaying = false;
			mciSendString(L"pause em3", NULL, 0, NULL);
		}
		else if (temp2 == 6)
		{
			settings();
		}
		else if (temp2 == 7)
		{
			resetmenu = false;
			keepplaying = false;
		}
		else
			continue;

		while (keepplaying == true) // starts when game is opened. main gameplay loop
		{
			if (lvlrdy == true)
			{
				cout << "Your character has a pending level up\n0 - Level up screen" << endl;
			}

			cout << "1 - Battle\n2 - View character\n3 - Save game\n4 - Main Menu\n5 - Exit game" << endl;
			cin >> mmc;
			system("CLS");
			switch (mmc)
			{
			case 0: if (lvlrdy == true)
				lvlup();
				break;
			case 1: ts = rand() % 2; // rng between two fight themes
				if (ts == 0)
				{
					mciSendString(L"pause maintheme", NULL, 0, NULL);
					mciSendString(L"seek fighttheme to start", NULL, 0, NULL);
					mciSendString(L"play fighttheme repeat", NULL, 0, NULL);
					play();
					mciSendString(L"pause fighttheme", NULL, 0, NULL);
					mciSendString(L"seek maintheme to start", NULL, 0, NULL);
					mciSendString(L"play maintheme repeat", NULL, 0, NULL);
				}
				else
				{
					mciSendString(L"pause maintheme", NULL, 0, NULL);
					mciSendString(L"seek fighttheme2 to start", NULL, 0, NULL);
					mciSendString(L"play fighttheme2 repeat", NULL, 0, NULL);
					play();
					mciSendString(L"pause fighttheme2", NULL, 0, NULL);
					mciSendString(L"seek maintheme to start", NULL, 0, NULL);
					mciSendString(L"play maintheme repeat", NULL, 0, NULL);
				}
				break;
			case 2: viewchar(1);
				break;
			case 3: savegame();
				break;
			case 4: keepplaying = false;
				break;
			case 5: keepplaying = false; // quite just gameplay loop
				resetmenu = false; // quite main menu loop
				break;
			case 6:
				dt = (rand() % 3); // item drop type
				mciSendString(L"seek itemdsfx to start", NULL, 0, NULL);
				mciSendString(L"play itemdsfx", NULL, 0, NULL);
				if (dt == 0)
				{
					weapon wd = createweapon();
					cout << "You've found " << wd.wname << "!" << endl;
					for (int i = 0; i < 5 - 1; i++) // sort inv by descending power
						for (int j = 0; j < 5 - i - 1; j++)
							if (weaponsarray[j].wcatt < weaponsarray[j + 1].wcatt)
							{
								weapon temp = weaponsarray[j];
								weaponsarray[j] = weaponsarray[j + 1];
								weaponsarray[j + 1] = temp;
							}
					if (wd.wcatt > weaponsarray[4].wcatt) // if dropped item is better than worse inventory item
						weaponsarray[4] = wd;
					for (int i = 0; i < 5 - 1; i++) // sort inv by descending power again with new item
						for (int j = 0; j < 5 - i - 1; j++)
							if (weaponsarray[j].wcatt < weaponsarray[j + 1].wcatt)
							{
								weapon temp = weaponsarray[j];
								weaponsarray[j] = weaponsarray[j + 1];
								weaponsarray[j + 1] = temp;
							}
				}
				if (dt == 1) // same as weapon drop
				{
					armor ad = createarmor();
					cout << "You've found " << ad.aname << "!" << endl;
					for (int i = 0; i < 5 - 1; i++)
						for (int j = 0; j < 5 - i - 1; j++)
							if (armorsarray[j].atotalstat < armorsarray[j + 1].atotalstat)
							{
								armor temp = armorsarray[j];
								armorsarray[j] = armorsarray[j + 1];
								armorsarray[j + 1] = temp;
							}
					if (ad.atotalstat > armorsarray[4].atotalstat)
						armorsarray[4] = ad;
					for (int i = 0; i < 5 - 1; i++)
						for (int j = 0; j < 5 - i - 1; j++)
							if (armorsarray[j].atotalstat < armorsarray[j + 1].atotalstat)
							{
								armor temp = armorsarray[j];
								armorsarray[j] = armorsarray[j + 1];
								armorsarray[j + 1] = temp;
							}
				}
				if (dt == 2) // same as weapon drop
				{
					helmet hd = createhelmet();
					cout << "You've found " << hd.hname << "!" << endl;
					for (int i = 0; i < 5 - 1; i++)
						for (int j = 0; j < 5 - i - 1; j++)
							if (helmetsarray[j].htotalstat < helmetsarray[j + 1].htotalstat)
							{
								helmet temp = helmetsarray[j];
								helmetsarray[j] = helmetsarray[j + 1];
								helmetsarray[j + 1] = temp;
							}
					if (hd.htotalstat > helmetsarray[4].htotalstat)
						helmetsarray[4] = hd;
					for (int i = 0; i < 5 - 1; i++)
						for (int j = 0; j < 5 - i - 1; j++)
							if (helmetsarray[j].htotalstat < helmetsarray[j + 1].htotalstat)
							{
								helmet temp = helmetsarray[j];
								helmetsarray[j] = helmetsarray[j + 1];
								helmetsarray[j + 1] = temp;
							}
				}
			default:
				break;
			}
		}
		keepplaying = true;
		mciSendString(L"pause maintheme", NULL, 0, NULL);
	} while (resetmenu == true);

	mciSendString(L"close maintheme", NULL, 0, NULL);
	mciSendString(L"close em3", NULL, 0, NULL);
	mciSendString(L"close fighttheme", NULL, 0, NULL);
	mciSendString(L"close fighttheme2", NULL, 0, NULL);
	mciSendString(L"close critsfx", NULL, 0, NULL);
	mciSendString(L"close itemdsfx", NULL, 0, NULL);
	mciSendString(L"close elitesfx", NULL, 0, NULL);
	mciSendString(L"close vsfx", NULL, 0, NULL);
	mciSendString(L"close dsfx", NULL, 0, NULL);
	return 0;
}