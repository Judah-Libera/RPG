//     RPG V2.6.2         newcontent.alteredcontent/balancing.backendchanges/bugs

//BUGS
//equipment names not read in from save file ???save inv structs seperate and put all in a character folder. inprogress - change drop vals in play() then test???
//unequip and droped weapon crashed after failed weapon name read

//EDITS
//merge catt and catlvlups into same variable.
//sfx need to be update to pvp.cpp

//FEATURES
//lvlupsfx
//real soundtrack
//boss theme(?dissonent theme? tremelo bass, e,f#,g. melody around a#. dim5th tritone)
//try fibonacci for late game scaling? < x^2 till x > 12
//renown title and name generation every 5 levels past 10
//colorize
//?story mode?
//?map/travel?
//have someone else put in multiple character slots

#pragma warning(disable: 4996)

#include "includes.h"

using namespace std;

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

struct weapon
{
	string wname = "";
	int wcatt = 0;
};
struct armor
{
	string aname = "";
	int amchp = 0;
	int aregen = 0;
	int atotalstat = 0;
};
struct helmet
{
	string hname = "";
	int hmchp = 0;
	int hregen = 0;
	int htotalstat = 0;
};
struct character
{
	string cclass = "";
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
	weapon weaponsarray[5]; //arrays for inv
	armor ae = { "", 0, 0 };
	armor armorsarray[5];
	helmet he = { "", 0, 0 };
	helmet helmetsarray[5];
};
character c; //active character
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
			wname1 = "Dueling Blades of ";
		else
			wname1 = "Battle Axe of ";

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
			hname1 = "Hood ";
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
			hname1 = "Cap ";
		else
			hname1 = "Hat ";
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
}

void play()
{
	cout << "When you see 'ATTACK #' hit the number then enter before times up" << endl;
	int rng = rand() % 5 + 6; //6-10 // ai diff
	int aihp = rng * aihpscaler();
	int numcheck = 10;
	int damage = 0;
	int dt = 4;

	bool boss = false; //testign inv saves - make true
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

			if (atttime <= .6) // crit
			{
				cntratt = 2; // don't cntr att on crit
				mciSendString(L"seek critsfx to start", NULL, 0, NULL);
				mciSendString(L"play critsfx", NULL, 0, NULL);
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
				character q = { "", 200, 200, 10, 0, 3, 0, 1, 0, difsel};
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
				cout << "LEVEL UP\n" << endl;
				lvlrdy = true;
			}
			if (boss == true)
			{
				int ld = (rand() % 2); //testign inv saves - make 1. default 2 // item drop chance
				if (ld == 1)
				{
					dt = (rand() % 3);//testign inv saves - make 0. defualt 3 // item drop type
					mciSendString(L"seek itemdsfx to start", NULL, 0, NULL);
					mciSendString(L"play itemdsfx", NULL, 0, NULL);
					if (dt == 0)
					{
						weapon wd = createweapon();
						cout << "You've found " << wd.wname << "!" << endl;
						for (int i = 0; i < 5 - 1; i++) // sort inv by descending power
							for (int j = 0; j < 5 - i - 1; j++)
								if (c.weaponsarray[j].wcatt < c.weaponsarray[j + 1].wcatt)
								{
									weapon temp = c.weaponsarray[j];
									c.weaponsarray[j] = c.weaponsarray[j + 1];
									c.weaponsarray[j + 1] = temp;
								}
						if (wd.wcatt > c.weaponsarray[4].wcatt) // if dropped item is better than worse inventory item
							c.weaponsarray[4] = wd;
					}
					if (dt == 1) // same as weapon drop
					{
						armor ad = createarmor();
						cout << "You've found " << ad.aname << "!" << endl;
						for (int i = 0; i < 5 - 1; i++)
							for (int j = 0; j < 5 - i - 1; j++)
								if (c.armorsarray[j].atotalstat < c.armorsarray[j + 1].atotalstat)
								{
									armor temp = c.armorsarray[j];
									c.armorsarray[j] = c.armorsarray[j + 1];
									c.armorsarray[j + 1] = temp;
								}
						if (ad.atotalstat > c.armorsarray[4].atotalstat)
							c.armorsarray[4] = ad;
					}
					if (dt == 2) // same as weapon drop
					{
						helmet hd = createhelmet();
						cout << "You've found " << hd.hname << "!" << endl;
						for (int i = 0; i < 5 - 1; i++)
							for (int j = 0; j < 5 - i - 1; j++)
								if (c.helmetsarray[j].htotalstat < c.helmetsarray[j + 1].htotalstat)
								{
									helmet temp = c.helmetsarray[j];
									c.helmetsarray[j] = c.helmetsarray[j + 1];
									c.helmetsarray[j + 1] = temp;
								}
						if (hd.htotalstat > c.helmetsarray[4].htotalstat)
							c.helmetsarray[4] = hd;
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
		cout << "\nhp - " << c.chp << "\nattack - " << c.catt + c.we.wcatt << "\n\n" << endl;
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
	if (c.dif == 2) // dispaly in red for HC character
		SetConsoleTextAttribute(hConsole, 4);
	cout << difsel << "<- difsel : c.dif->" << c.dif << endl;
	cout << "Class - " << c.cclass << endl;
	cout << "HP - " << c.chp << "/" << c.mchp + c.ae.amchp + c.he.hmchp << endl;
	cout << "Regeneration - " << c.regen + c.ae.aregen + c.he.hregen << endl;
	cout << "attack - " << c.catt + c.we.wcatt << endl;
	cout << "Expierience - " << c.cxp << "/" << (int)xpcostscaler() << endl;
	cout << "Level - " << (int)c.lvl << "\n\n" << endl;
	cout << "\nCurrent Weapon: " << c.we.wname << "\nCurrent Armor: " << c.ae.aname << "\nCurrent Helmet: " << c.he.hname << endl;
	if (c.dif == 2) // put back normal
		SetConsoleTextAttribute(hConsole, 15);

	if (pac == 1) // inventory managment option. not sure when viewchar is called without it
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
			do
			{
				ri = false;
				system("CLS");
				cout << "\nCurrent Weapon: " << c.we.wname << "\nCurrent Armor: " << c.ae.aname << "\nCurrent Helmet: " << c.he.hname << endl;

				cout << "\nA - Weapons in inventory." << endl;
				for (int i = 0; i < 5; i++)
				{
					if (c.weaponsarray[i].wname == "")
						cout << i + 1 << " - " << "empty" << endl;
					else
						cout << i + 1 << " - " << c.weaponsarray[i].wname << " - " << c.weaponsarray[i].wcatt << " att" << endl;
				}
				cout << "\nB - Armors in inventory." << endl;
				for (int i = 0; i < 5; i++)
				{
					if (c.armorsarray[i].aname == "")
						cout << i + 1 << " - " << "empty" << endl;
					else
						cout << i + 1 << " - " << c.armorsarray[i].aname << " - " << c.armorsarray[i].amchp << " hp" << " - " << c.armorsarray[i].aregen << " regen" << endl;
				}
				cout << "\nC - Helmets in inventory." << endl;
				for (int i = 0; i < 5; i++)
				{
					if (c.helmetsarray[i].hname == "")
						cout << i + 1 << " - " << "empty" << endl;
					else
						cout << i + 1 << " - " << c.helmetsarray[i].hname << " - " << c.helmetsarray[i].hmchp << " hp" << " - " << c.helmetsarray[i].hregen << " regen" << endl;
				}

				cout << "\nTo switch items enter the letter of the item type you would like to replace, or D to delete/unequip. Enter 0 at any time to exit." << endl;
				cin >> mc;
				if (mc == 'a' || mc == 'A')
				{
					cout << "Enter the number of the weapon you would like to equip." << endl;
					cin >> mc2;
					if (mc2 == 1)
						c.we = c.weaponsarray[0];
					else if (mc2 == 2)
						c.we = c.weaponsarray[1];
					else if (mc2 == 3)
						c.we = c.weaponsarray[2];
					else if (mc2 == 4)
						c.we = c.weaponsarray[3];
					else if (mc2 == 5)
						c.we = c.weaponsarray[4];
					else
						skip = true;
				}
				else if (mc == 'b' || mc == 'B')
				{
					cout << "Enter the number of the armor you would like to equip." << endl;
					cin >> mc2;
					if (mc2 == 1)
						c.ae = c.armorsarray[0];
					else if (mc2 == 2)
						c.ae = c.armorsarray[1];
					else if (mc2 == 3)
						c.ae = c.armorsarray[2];
					else if (mc2 == 4)
						c.ae = c.armorsarray[3];
					else if (mc2 == 5)
						c.ae = c.armorsarray[4];
					else
						skip = true;
				}
				else if (mc == 'c' || mc == 'C')
				{
					cout << "Enter the number of the helmet you would like to equip." << endl;
					cin >> mc2;
					if (mc2 == 1)
						c.he = c.helmetsarray[0];
					else if (mc2 == 2)
						c.he = c.helmetsarray[1];
					else if (mc2 == 3)
						c.he = c.helmetsarray[2];
					else if (mc2 == 4)
						c.he = c.helmetsarray[3];
					else if (mc2 == 5)
						c.he = c.helmetsarray[4];
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
							c.weaponsarray[0] = { "",0 };
						else if (mc5 == 2)
							c.weaponsarray[0] = { "",0 };
						else if (mc5 == 3)
							c.weaponsarray[0] = { "",0 };
						else if (mc5 == 4)
							c.weaponsarray[0] = { "",0 };
						else if (mc5 == 5)
							c.weaponsarray[0] = { "",0 };
						else if (mc5 == 6)
						{
							for (int i = 0; i < 5 - 1; i++)
								for (int j = 0; j < 5 - i - 1; j++)
									if (c.weaponsarray[j].wcatt < c.weaponsarray[j + 1].wcatt)
									{
										weapon temp = c.weaponsarray[j];
										c.weaponsarray[j] = c.weaponsarray[j + 1];
										c.weaponsarray[j + 1] = temp;
									}
							c.weaponsarray[4] = c.we;
							c.we = { "",0 };
						}
					}
					else if (mc4 == 'b' || mc == 'B')
					{
						cout << "Enter the number of the armor you would like to delete or enter 6 to unequip your current armor." << endl;
						cin >> mc5;
						if (mc5 == 1)
							c.armorsarray[0] = { "",0 };
						else if (mc5 == 2)
							c.armorsarray[0] = { "",0 };
						else if (mc5 == 3)
							c.armorsarray[0] = { "",0 };
						else if (mc5 == 4)
							c.armorsarray[0] = { "",0 };
						else if (mc5 == 5)
							c.armorsarray[0] = { "",0 };
						else if (mc5 == 6)
						{
							for (int i = 0; i < 5 - 1; i++)
								for (int j = 0; j < 5 - i - 1; j++)
									if (c.armorsarray[j].atotalstat < c.armorsarray[j + 1].atotalstat)
									{
										armor temp = c.armorsarray[j];
										c.armorsarray[j] = c.armorsarray[j + 1];
										c.armorsarray[j + 1] = temp;
									}
							c.armorsarray[4] = c.ae;
							c.ae = { "",0 };
						}
					}
					else if (mc4 == 'c' || mc == 'C')
					{
						cout << "Enter the number of the helmet you would like to delete or enter 6 to unequip your current helmet." << endl;
						cin >> mc5;
						if (mc5 == 1)
							c.helmetsarray[0] = { "",0 };
						else if (mc5 == 2)
							c.helmetsarray[0] = { "",0 };
						else if (mc5 == 3)
							c.helmetsarray[0] = { "",0 };
						else if (mc5 == 4)
							c.helmetsarray[0] = { "",0 };
						else if (mc5 == 5)
							c.helmetsarray[0] = { "",0 };
						else if (mc5 == 6)
						{
							for (int i = 0; i < 5 - 1; i++)
								for (int j = 0; j < 5 - i - 1; j++)
									if (c.helmetsarray[j].htotalstat < c.helmetsarray[j + 1].htotalstat)
									{
										helmet temp = c.helmetsarray[j];
										c.helmetsarray[j] = c.helmetsarray[j + 1];
										c.helmetsarray[j + 1] = temp;
									}
							c.helmetsarray[4] = c.he;
							c.he = { "",0 };
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
			cout << "Class - " << c.cclass << endl;
			cout << "HP - " << c.chp << "/" << c.mchp + c.ae.amchp + c.he.hmchp << endl;
			cout << "Regeneration - " << c.regen + c.ae.aregen + c.he.hregen << endl;
			cout << "attack - " << c.catt + c.we.wcatt << endl;
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
	FILE* ofpb = fopen("char/chardata.bin", "wb"); //save char
	fwrite(&c, sizeof(character), 1, ofpb);
	fclose(ofpb);

	FILE* ofpb2 = fopen("char/weapons.bin", "wb"); //save weapons inventory
	for (int i = 0; i < 5; i++)		
		fwrite(&c.weaponsarray[i], sizeof(weapon), 1, ofpb2);
	fclose(ofpb2);
	//manual save all enrties in equipments arrays as bin(still loop and save individually). hae to save equiped items seperat also?
	system("CLS");
}

void loadchar()
{
	FILE* ifpb = fopen("char/chardata.bin", "rb"); //load char
	fread(&c, sizeof(character), 1, ifpb);
	fclose(ifpb);

	FILE* ifpb2 = fopen("char/weapons.bin", "rb"); //stil ltrying to fix equipment saving/loading
	for (int i = 0; i < 5; i++)
		fread(&c.weaponsarray[i], sizeof(weapon), 1, ifpb2);
	fclose(ifpb2);
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
		cout << "1 - New Character Difficulty\n2 - Toggle Full Screen launch\n3 - Music Volume\n4 - Sound Effects Volume\n5 - exit" << endl;

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
		case 5: vi = true;
			keepplaying = false;
			break;
		default: vi = false;
			break;
		}
	} while (vi != true);
	system("CLS");

	FILE* ofp = fopen("settings.txt", "w"); //update settigns file
	fprintf(ofp, "FullScreen %d\nStart Difficulty %d\nMusic Volume %d\nEffects Volume %d", FullScreen, difsel, mvolume, sfxvolume);
	fclose(ofp);
	mciSendString(L"close elitesfx", NULL, 0, NULL);
}

int main()
{
	//SetConsoleTextAttribute(hConsole, 15);
	mciSendString(L"open audio/titletheme.mp3 type mpegvideo alias em3", NULL, 0, NULL); //open all audio files gonna be needed. 
	mciSendString(L"open audio/fighttheme.mp3 type mpegvideo alias fighttheme", NULL, 0, NULL);
	mciSendString(L"open audio/fighttheme2.mp3 type mpegvideo alias fighttheme2", NULL, 0, NULL);
	mciSendString(L"open audio/maintheme.mp3 type mpegvideo alias maintheme", NULL, 0, NULL);
	mciSendString(L"open audio/titletheme.mp3 type mpegvideo alias titletheme", NULL, 0, NULL);
	mciSendString(L"open audio/critsfx.mp3 type mpegvideo alias critsfx", NULL, 0, NULL);
	mciSendString(L"open audio/itemdsfx.mp3 type mpegvideo alias itemdsfx", NULL, 0, NULL);
	mciSendString(L"open audio/elitesfx.mp3 type mpegvideo alias elitesfx", NULL, 0, NULL);
	mciSendString(L"open audio/vsfx.mp3 type mpegvideo alias vsfx", NULL, 0, NULL);
	mciSendString(L"open audio/dsfx.mp3 type mpegvideo alias dsfx", NULL, 0, NULL);
	FILE* ifp = fopen("settings.txt", "r"); //read in settigns
	fscanf(ifp, "FullScreen %d\n", &FullScreen);
	fscanf(ifp, "Start Difficulty %d\n", &difsel);
	fscanf(ifp, "Music Volume %d\n", &mvolume);
	fscanf(ifp, "Effects Volume %d", &sfxvolume);
	fclose(ifp);

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
			case 1: c = { "Orc", 200, 200, 10, 0, 3, 0, 1, 0};
				  c.dif = difsel;
				  break;
			case 2: c = { "Human", 100, 100, 20, 0, 3, 0, 1, 0};
				  c.dif = difsel;
				  break;
			case 3: c = { "Elf", 50, 50, 40, 0, 3, 0, 1, 0};
				  c.dif = difsel;
				  break;
			default:
				break;
			}
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
			cout << "To win combat, hit your opponent before they hit you.\nAttacking is done by entering the number shown next to attack: then hitting the enter key.\nAttacks that take under 1 second will deal damage increasing with how fast you were.\nAttacks that miss or hit too late will result in your opponent winning the round.\n\nAi hp and damage decided through rng that scales with player level.\n\nXP gain and level up requirements also scaled with player level.\n\nThere's a 1/3 chance half of the enemies damage will be dealt as counter attack upon successful hit.\n\nAfter defeating an enemy, using the same mechanics as attacking, you are able to regen hp by an amount scaling\nwith your max hp as well your regeneration stat.\n\ndon't miss your chance to level up, if you earn another one with the previous still pending you'll loose it.\nHard difficulty will erase save data of a character that dies but scroe will be doubled.\n" << endl;
			system("pause");
			system("CLS");
			keepplaying = false;
			mciSendString(L"pause em3", NULL, 0, NULL);
		}
		else if (temp2 == 4)
		{ // basically a copy of play()
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