//     RPG V4.1.0				newcontent.alteredcontent/balancing.backendchanges/bugs

//IP
//dnugeons.cpp functionality changes whe naccessed from TLE. those changes are commented inside of dungeons.cpp where dungeontype is proccessed.

//BUGS
//gameplay loop stuck infinite looping after entering name as more that one word. (idk if it breaks just setting it or also on gameload or other uses of it). forces name that will work rn

//TESTING
//dungeon selection moved to source.cpp
//TLE access from game and dungeon/dialogue
//won't load an empty save
//audio
//highscore table - if issue present its likely in saving
//lots of commented code

//EDITS
//merge catt and catlvlups into same variable. //if it ain't broke dont fix it tho
//update pvp.cpp - sfx - char loads

//FEATURES
//add hlwknt st3
//multiple save games. read folders in savedata folder, each character gets a folder with a number, user enters number to load filepath using that number. save game into folder with what ever number is entered.
//boss theme(?dissonent theme? tremelo bass, e,f#,g. melody around a#. dim5th tritone)
//colorize
//?map/travel? probably means reworkign drops to base off level. level scaling would need to be off location level. ascii map with letter for location. unlock through clevel? final dungeon for last title?

#include "includes.h"
using namespace std;

static int titlescreen();
static void viewhscores();
static void updatehscores();
static void lvlup();
static void play();
static void killchar();
static void viewchar(int pac);
static void savegame();
static void loadchar();
static void settings();
static weapon createweapon();
static armor createarmor();
static helmet createhelmet();

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
bool keepplaying = true;
bool lvlrdy = false;
int difsel = 1; // 1=normal, 2=hardcore, 5920000737=cheateasy
int FullScreen = 1; // 1=fs, 0=windowed
int mvolume = 1000; // = 0-10 * 100
int sfxvolume = 1000; // ^^^
int soundtracknum = 1;
int dungeontype = 1; //0 - auto-gen off. 1 - on

character c = {};
weapon weaponsarray[5]; //arrays for inv
armor armorsarray[5];
helmet helmetsarray[5];

static const double aiattscaler() { return (.035 * pow(c.lvl, 2)) + (.1 * (double)c.lvl) + 1; } // uses different variables in other files, so they get there own set.
static const double aihpscaler() { return (.25 * pow(c.lvl, 1.9)) + (2 * (double)c.lvl) + 3; }
static const double cattscaler() { return (c.catt + (.6 * c.attlvlups)); } //NOT USED IN DAMAGE CALCULATION. influence the effect of lvlups on natural attack //(c.catt + (.12 * (double)pow(c.attlvlups, 1.5) + 1.2 * (double)c.attlvlups + 1.5)); }//old
static const double mchpscaler() { return ((double)c.mchp * 1.08 + (((double)c.ae.amchp + (double)c.he.hmchp) * 1.04) + 5); }
static const double xpgainscaler() { return (((double)(rand() % 3) + 2)) * (1 + (double)c.lvl / 2); }
static const double xpcostscaler() { return ((.1 * (double)pow(c.lvl, 2)) + (5 * (double)c.lvl) + 5); }
static const double regenscaler() { return (.8 * (double)(5 + c.regen + c.ae.aregen + c.he.hregen)) * ((double)c.mchp / 15); } // * (1 - atttime)

static weapon createweapon()
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

static armor createarmor()
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

static helmet createhelmet()
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

static int titlescreen()
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

	while (resetmenu == true)
	{
		resetmenu = false;

		FILE* fptr = fopen("asciiart.txt", "r"); // this chunk does the title art
		char read_string[128];
		while (fgets(read_string, sizeof(read_string), fptr) != NULL)
			printf("%s", read_string);
		fclose(fptr);

		//int choice;
		cout << "\n\n";
		system("pause");
		/*printf("\n\n\npress enter to continue\n"); //pvp is to outdatted to be remotely functional
		if (cin.get() == '1')
		{
			choice = pvp(); //launch pvp script. will end returning 1 if exits game from pvp()
			if (choice == 1)
				return 1;
			else if (choice == 0)
				resetmenu = true;
			else {}
		}*/
	}
	cin.seekg(0, ios::end); // no idea what this does but i'd rather not cuase permanate damage tryign to figure it out
	cin.clear();
	return 0;
}

static void viewhscores()
{
	string line;
	ifstream sdata;
	string n;
	sdata.open("hscores.txt"); //just prints lines from hscores

	if (c.name == "") //to be able to accuratly compare with line wwhen highlighting player
		if (c.cclass == "Human")
			n = "A Human of no renown";
		else
			n = "An " + c.cclass + " of no renown";
	else
		n = c.nametitle;

	cout << "High Scores" << endl;
	while (getline(sdata, line))
	{
		//cout << (to_string(c.score) + "\t" + n) << endl; // for testing if comparison below is brokens
		if (line == (to_string(c.score) + "\t" + n)) // if record matches this games then off color
		{
			SetConsoleTextAttribute(hConsole, 7);
			cout << line << endl;
			SetConsoleTextAttribute(hConsole, 15);
		}
		else
			cout << line << endl;
	}
	sdata.close();
}

static void updatehscores()
{
	if (c.dif == 2) // double score for hardcore
		c.score = c.score * 2;
	int scorearray[11] = { 0,0,0,0,0,0,0,0,0,0,0 }; //place holders
	string names[11] = { "","","","","","","","","","","" };

	string line;
	ifstream sdatai;
	sdatai.open("hscores.txt");

	int counter = 0;
	while (getline(sdatai, line, '\t')) //reads lines into place holders. data is stored in descending order and thus read in likewise
	{
		
		scorearray[counter] = stoi(line);
		getline(sdatai, line);
		names[counter] = line;
		counter++;
	}
	sdatai.close();

		scorearray[10] = c.score; // put score into 11th index, should be empty as only 10 are stored.
		if (c.nametitle == "")
			if (c.cclass == "Human")
				names[10] = "A " + c.cclass + " of no renown";
			else
				names[10] = "An " + c.cclass + " of no renown";
		else
			names[10] = c.nametitle;
		for (int i = 11; i != 0; --i) //descending bubble
		{
			for (int j = 11; --j != 11 - i;) //increment handled in condition
			{
				if (scorearray[j - 1] < scorearray[j])
				{
					deadlibrary::numswap(&scorearray[j], &scorearray[j - 1]);
					deadlibrary::strswap(&names[j], &names[j - 1]);
				}
			}
		}
		ofstream sdatao;
		sdatao.open("hscores.txt"); //save sorted data
		sdatao << scorearray[0] << "\t" << names[0] << "\n" << scorearray[1] << "\t" << names[1] << "\n" << scorearray[2] << "\t" << names[2] << "\n" << scorearray[3] << "\t" << names[3] << "\n" << scorearray[4] << "\t" << names[4] << "\n" << scorearray[5] << "\t" << names[5] << "\n" << scorearray[6] << "\t" << names[6] << "\n" << scorearray[7] << "\t" << names[7] << "\n" << scorearray[8] << "\t" << names[8] << "\n" << scorearray[9] << "\t" << names[9];
		sdatao.close();
}

static void lvlup()
{
	mciSendString(L"open audio/sfx/lvlupsfx.mp3 type mpegvideo alias lvlupsfx", NULL, 0, NULL);
	mciSendString(L"seek lvlupsfx to start", NULL, 0, NULL);
	mciSendString(L"play lvlupsfx", NULL, 0, NULL);
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
	c.lvl++;
	if (c.cxp < (int)xpcostscaler)
		lvlrdy = false;

	if (c.lvl == 5)
	{
		cout << "Your journey as a hero has begun. Who will you be remembered as?" << endl;
		cin >> c.name;
		remove(c.name.begin(), c.name.end(), ' ');
		system("CLS");
		cout << "And thus.";
		Sleep(500);
		cout << ".";
		Sleep(500);
		cout << "." << endl;
		Sleep(1000);
		cout << "A ";
		Sleep(250);
		cout << "Legend ";
		Sleep(250);
		cout << "was ";
		Sleep(250);
		cout << "born\n\n" << endl;
		Sleep(1000);
		system("pause");
		system("CLS");
		c.nametitle = c.name + " the distinguished";
	}
	if (c.lvl == 10)
	{
		cout << "Congratulations Hero! your prestige has grown" << endl;
		Sleep(500);
		system("pause");
		system("CLS");
		c.nametitle = c.name + " the prestigious";
	}
	if (c.lvl == 15)
	{
		cout << "Congratulations Hero! your fame is spreading" << endl;
		Sleep(500);
		system("pause");
		system("CLS");
		c.nametitle = c.name + " the renowned";
	}
	if (c.lvl == 20)
	{
		cout << "Congratulations Hero! your an icon to those following your path" << endl;
		Sleep(500);
		system("pause");
		system("CLS");
		c.nametitle = "the glorious " + c.name;
	}
	if (c.lvl == 25)
	{
		cout << "Congratulations Hero! you've become a legend to your followers" << endl;
		Sleep(500);
		system("pause");
		system("CLS");
		c.nametitle = c.name + " the legend";
	}
	if (c.lvl == 30)
	{
		cout << "Congratulations Hero! all have come to know your legend" << endl;
		Sleep(500);
		system("pause");
		system("CLS");
		c.nametitle = c.name + " the fabled";
	}
	if (c.lvl == 35)
	{
		cout << "Congratulations Hero! you've become exalted above humanity" << endl;
		Sleep(500);
		system("pause");
		system("CLS");
		c.nametitle = c.name + " the exalted";
	}
	if (c.lvl == 40)
	{
		cout << "Congratulations Hero! you've transcended all which has been" << endl;
		Sleep(500);
		system("pause");
		system("CLS");
		c.nametitle = c.name + " the immortal";
	}
	if (c.lvl == 50)
	{
		cout << "There is no higher honor. There is no higher power. All has become far beneath you." << endl;
		Sleep(500);
		cout << ".";
		Sleep(500);
		cout << ".";
		Sleep(500);
		cout << ".";
		Sleep(500);
		cout << "." << endl;;
		Sleep(500);
		cout << "You ";
		Sleep(250);
		cout << "are ";
		Sleep(250);
		cout << "the ";
		Sleep(500);
		cout << "Immortal ";
		Sleep(750);
		cout << "High ";
		Sleep(750);
		cout << "King " << endl;
		Sleep(1500);
		cout << "\n\n\n";
		system("pause");
		system("CLS");
		c.nametitle == "Immortal High King " + c.name;
	}
	mciSendString(L"close elitesfx", NULL, 0, NULL);
}

static void play()
{
	cout << "When you see 'ATTACK #' hit the number then enter before times up" << endl;
	int rng = rand() % 5 + 6; //6-10 // ai hp rng
	int aihp = rng * aihpscaler();
	int numcheck = 10;
	int damage = 0;
	int dt = 4;
	int totalmaxhp = c.mchp + c.ae.amchp + c.he.hmchp;

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
			//mciSendString(L"seek dsfx to start", NULL, 0, NULL);
			//mciSendString(L"play dsfx", NULL, 0, NULL);

			cout << "hp - 0\nYour garbage and don't deserve a second chance looser. Your score was " << c.score << ".\n\n" << endl;
			cout << "ai hp - " << aihp << "\nai attack - " << aiatt << "\n\n" << endl;
			
			if (c.dif != 0) // don't get score saved for easy
				updatehscores();
			if (c.dif == 2) // if hardcore, creates empty char to overwrite save data
			{
				c = { "", "", "", 0, 0, 0, 0, 0, 0, 0, 0, difsel};
				savegame();
				system("CLS");
			}
			system("pause");
			system("CLS");
			viewhscores();
			system("pause");
			exit(0);

			/*
			mciSendString(L"pause fighttheme", NULL, 0, NULL);
			mciSendString(L"pause fighttheme2", NULL, 0, NULL);
			mciSendString(L"seek maintheme to start", NULL, 0, NULL);
			mciSendString(L"play maintheme repeat", NULL, 0, NULL);
			mciSendString(L"pause maintheme", NULL, 0, NULL); //close game
			mciSendString(L"close maintheme", NULL, 0, NULL);
			mciSendString(L"close em3", NULL, 0, NULL);
			mciSendString(L"close fighttheme", NULL, 0, NULL);
			mciSendString(L"close fighttheme2", NULL, 0, NULL);
			mciSendString(L"close practicetheme", NULL, 0, NULL);
			mciSendString(L"close critsfx", NULL, 0, NULL);
			mciSendString(L"close itemdsfx", NULL, 0, NULL);
			mciSendString(L"close elitesfx", NULL, 0, NULL);
			mciSendString(L"close vsfx", NULL, 0, NULL);
			mciSendString(L"close dsfx", NULL, 0, NULL);
			mciSendString(L"close lvlupsfx", NULL, 0, NULL);
			*/
		}
		if (aihp < 1) // enemy dead
		{
			mciSendString(L"seek vsfx to start", NULL, 0, NULL);
			mciSendString(L"play vsfx", NULL, 0, NULL);

			int xpgain = xpgainscaler();
			c.cxp = c.cxp + xpgain;
			cout << "victory! you gained " << xpgain << " xp.\nCurrent xp: " << c.cxp << "/" << (int)xpcostscaler() << endl;
			cout << "\nhp remaining - " << c.chp << "/" << totalmaxhp << "\n" << endl;
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
						rpglib::sortweapon(weaponsarray);
						if (wd.wcatt > weaponsarray[4].wcatt) // if dropped item is better than worse inventory item
							weaponsarray[4] = wd;
						rpglib::sortweapon(weaponsarray);
					}
					if (dt == 1) // same as weapon drop
					{
						armor ad = createarmor();
						cout << "You've found " << ad.aname << "!" << endl;
						rpglib::sortarmor(armorsarray);
						if (ad.atotalstat > armorsarray[4].atotalstat)
							armorsarray[4] = ad;
						rpglib::sortarmor(armorsarray);
					}
					if (dt == 2) // same as weapon drop
					{
						helmet hd = createhelmet();
						cout << "You've found " << hd.hname << "!" << endl;
						rpglib::sorthelmet(helmetsarray);
						if (hd.htotalstat > helmetsarray[4].htotalstat)
							helmetsarray[4] = hd;
						rpglib::sorthelmet(helmetsarray);
					}
				}
			}
			system("pause");
			system("CLS");

			int sucheal = (rand() % 6); // same as attack but heals instead and won't wait quite as long
			cout << "Rest and heal. current hp - " << c.chp << "/" << totalmaxhp << endl;
			system("pause");
			system("CLS");
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
				if (c.chp > totalmaxhp)
					c.chp = totalmaxhp;
			}
			else
				cout << "Your actually throwing, that was too important to choke this hard." << endl;
			system("pause");
			system("CLS");

			break;
		}

		cout << "\n\nhp - " << c.chp << "/" << totalmaxhp << "\nattack - " << (int)((double)c.catt + (c.we.wcatt + ((double)c.we.wcatt/3) * (((double)c.lvl - 1) * .5))) << "\n\n" << endl;
		cout << "ai hp - " << aihp << "\nai attack - " << aiatt << "\n" << endl;		
		cout << "1 - continue attacking\n2 - retreat" << endl;
		int temp3;
		cin >> temp3;
		if (temp3 == 2)
			keepatt = false;
		system("CLS");
	}
}

static void killchar()
{
	mciSendString(L"seek dsfx to start", NULL, 0, NULL);
	mciSendString(L"play dsfx", NULL, 0, NULL);

	if (c.dif != 0) // don't get score saved for easy
		updatehscores();
	if (c.dif == 2) // if hardcore, creates empty char to overwrite save data
	{
		c = { "", "", "", 0, 0, 0, 0, 0, 0, 0, 0, difsel };
		savegame();
		//system("CLS");
	}
	mciSendString(L"pause fighttheme", NULL, 0, NULL);//can probably just add a pause here if this ever comes up with something else playing
	mciSendString(L"pause fighttheme2", NULL, 0, NULL);
	mciSendString(L"seek maintheme to start", NULL, 0, NULL);
	mciSendString(L"play maintheme repeat", NULL, 0, NULL);
	system("pause");
	system("CLS");
	viewhscores();
	system("pause");
	mciSendString(L"pause maintheme", NULL, 0, NULL); //close game
	mciSendString(L"close maintheme", NULL, 0, NULL);
	mciSendString(L"close em3", NULL, 0, NULL);
	mciSendString(L"close fighttheme", NULL, 0, NULL);
	mciSendString(L"close fighttheme2", NULL, 0, NULL);
	mciSendString(L"close practicetheme", NULL, 0, NULL);
	mciSendString(L"close critsfx", NULL, 0, NULL);
	mciSendString(L"close itemdsfx", NULL, 0, NULL);
	mciSendString(L"close elitesfx", NULL, 0, NULL);
	mciSendString(L"close vsfx", NULL, 0, NULL);
	mciSendString(L"close dsfx", NULL, 0, NULL);
	mciSendString(L"close lvlupsfx", NULL, 0, NULL);
	exit(0);
}

static void viewchar(int pac)
{
	if (c.dif == 2) // display in red for HC character
		SetConsoleTextAttribute(hConsole, 4);
	if (c.name == "") //show name with title if applicable
		if (c.cclass == "Human")
			cout << "A " << c.cclass << endl;
		else
			cout << "An " << c.cclass << endl;
	else
		cout << c.nametitle << endl;
	cout << "HP - " << c.chp << "/" << c.mchp + c.ae.amchp + c.he.hmchp << endl;
	cout << "Regeneration - " << c.regen + c.ae.aregen + c.he.hregen << endl;
	cout << "attack - " << c.catt << " : weapon - " << (int)(c.we.wcatt + ((double)c.we.wcatt / 3) * (((double)c.lvl - 1) * .5)) << endl;
	cout << "Expierience - " << c.cxp << "/" << (int)xpcostscaler() << endl;
	cout << "Level - " << (int)c.lvl << "\n" << endl;
	cout << "\nCurrent Weapon: " << c.we.wname << "\nCurrent Armor: " << c.ae.aname << "\nCurrent Helmet: " << c.he.hname << "\n" << endl;
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
			armor emptyarmor = { "", 0, 0, 0, };
			helmet emptyhelmet = { "", 0, 0, 0 };
			do
			{
				ri = false;
				system("CLS");
				cout << "\nCurrent Weapon: " << c.we.wname << " - " << c.we.wcatt << " att" << "\nCurrent Armor: " << c.ae.aname << " - " << c.ae.amchp << " hp - " << c.ae.aregen << " regen" << "\nCurrent Helmet: " << c.he.hname << " - " << c.he.hmchp << " hp - " << c.he.hregen << " regen" << endl;

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
					ri = true;
					cout << "Enter the number of the weapon you would like to equip." << endl;
					cin >> mc2;
					if (mc2 >= 1 && mc2 <= 5)
						rpglib::swapweapon(c.we, weaponsarray[mc2 - 1]);
					/*else if (mc2 == 2)
						deadlibrary::swapweapon(c.we, weaponsarray[1]);
					else if (mc2 == 3)
						deadlibrary::swapweapon(c.we, weaponsarray[2]);
					else if (mc2 == 4)
						deadlibrary::swapweapon(c.we, weaponsarray[3]);
					else if (mc2 == 5)
						deadlibrary::swapweapon(c.we, weaponsarray[4]);*/
					else
						skip = true;
					rpglib::sortweapon(weaponsarray);
				}
				else if (mc == 'b' || mc == 'B')
				{
					ri = true;
					cout << "Enter the number of the armor you would like to equip." << endl;
					cin >> mc2;
					if (mc2 >= 1 && mc2 <= 5)
						rpglib::swaparmor(c.ae, armorsarray[mc2 - 1]);
					/*else if (mc2 == 2)
						deadlibrary::swaparmor(c.ae, armorsarray[1]);
					else if (mc2 == 3)
						deadlibrary::swaparmor(c.ae, armorsarray[2]);
					else if (mc2 == 4)
						deadlibrary::swaparmor(c.ae, armorsarray[3]);
					else if (mc2 == 5)
						deadlibrary::swaparmor(c.ae, armorsarray[4]);*/
					else
						skip = true;
					rpglib::sortarmor(armorsarray);
				}
				else if (mc == 'c' || mc == 'C')
				{
					ri = true;
					cout << "Enter the number of the helmet you would like to equip." << endl;
					cin >> mc2;
					if (mc2 >= 1 && mc2 <= 5)
						rpglib::swaphelmet(c.he, helmetsarray[mc2 - 1]);
					/*else if (mc2 == 2)
						deadlibrary::swaphelmet(c.he, helmetsarray[1]);
					else if (mc2 == 3)
						deadlibrary::swaphelmet(c.he, helmetsarray[2]);
					else if (mc2 == 4)
						deadlibrary::swaphelmet(c.he, helmetsarray[3]);
					else if (mc2 == 5)
						deadlibrary::swaphelmet(c.he, helmetsarray[4]);*/
					else
						skip = true;
					rpglib::sorthelmet(helmetsarray);
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
						if (mc5 >= 1 && mc5 <= 5)
							weaponsarray[mc5 - 1] = emptyweapon;
						/*else if (mc5 == 2)
							weaponsarray[1] = emptyweapon;
						else if (mc5 == 3)
							weaponsarray[2] = emptyweapon;
						else if (mc5 == 4)
							weaponsarray[3] = emptyweapon;
						else if (mc5 == 5)
							weaponsarray[4] = emptyweapon;*/
						else if (mc5 == 6)
						{
							weaponsarray[4] = c.we; // already sorted so worse item is in last slot
							c.we = emptyweapon;
							rpglib::sortweapon(weaponsarray);
						}
					}
					else if (mc4 == 'b' || mc == 'B')
					{
						cout << "Enter the number of the armor you would like to delete or enter 6 to unequip your current armor." << endl;
						cin >> mc5;
						if (mc5 >= 1 && mc5 <= 5)
							armorsarray[mc5 - 1] = emptyarmor;
						/*else if (mc5 == 2)
							armorsarray[1] = emptyarmor;
						else if (mc5 == 3)
							armorsarray[2] = emptyarmor;
						else if (mc5 == 4)
							armorsarray[3] = emptyarmor;
						else if (mc5 == 5)
							armorsarray[4] = emptyarmor;*/
						else if (mc5 == 6)
						{
							armorsarray[4] = c.ae;
							c.ae = emptyarmor;
							rpglib::sortarmor(armorsarray);
						}
					}
					else if (mc4 == 'c' || mc == 'C')
					{
						cout << "Enter the number of the helmet you would like to delete or enter 6 to unequip your current helmet." << endl;
						cin >> mc5;
						if (mc5 >= 1 && mc5 <= 5)
							helmetsarray[mc5 - 1] = emptyhelmet;
						/*else if (mc5 == 2)
							helmetsarray[1] = emptyhelmet;
						else if (mc5 == 3)
							helmetsarray[2] = emptyhelmet;
						else if (mc5 == 4)
							helmetsarray[3] = emptyhelmet;
						else if (mc5 == 5)
							helmetsarray[4] = emptyhelmet;*/
						else if (mc5 == 6)
						{
							helmetsarray[4] = c.he;
							c.he = emptyhelmet;
							rpglib::sorthelmet(helmetsarray);
						}
					}
					else
						skip = true;
				}
			} while (ri == true);
		}
		else
			skip = true;

		if (skip == false)
		{
			system("CLS");
			if (c.name == "") //show name with title if applicable
				if (c.cclass == "Human")
					cout << "A " << c.cclass << endl;
				else
					cout << "An " << c.cclass << endl;
			else
				cout << c.nametitle << endl;
			cout << "HP - " << c.chp << "/" << c.mchp + c.ae.amchp + c.he.hmchp << endl;
			cout << "Regeneration - " << c.regen + c.ae.aregen + c.he.hregen << endl;
			cout << "attack - " << c.catt << " : weapon - " << (int)(c.we.wcatt + ((double)c.we.wcatt / 3) * (((double)c.lvl - 1) * .5)) << endl;
			cout << "Expierience - " << c.cxp << "/" << (int)xpcostscaler() << endl;
			cout << "Level - " << c.lvl << "\n" << endl;
			cout << "\nCurrent Weapon: " << c.we.wname << "\nCurrent Armor: " << c.ae.aname << "\nCurrent Helmet: " << c.he.hname << "\n" << endl;
			system("pause");
		}
	}
	system("CLS");
}

static void savegame()
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

	ofstream out4("char/chardata.txt"); //cclass is one word so it can be read in like the other vars
	out4 << c.name << "\n" << c.nametitle << "\n" << c.cclass << " " << c.chp << " " << c.mchp << " " << c.catt << " " << c.attlvlups << " " << c.regen << " " << c.cxp << " " << c.lvl << " " << c.score << " " << c.dif << "\n";
	
	out1.close();
	out2.close();
	out3.close();
	out4.close();

	system("CLS");
}

static void loadchar()
{
	ifstream infile;
	infile.open("char/chardata.txt");
	string textline;
	getline(infile, c.name);
	getline(infile, c.nametitle);
	getline(infile, textline);
	istringstream data(textline);
	data >> c.cclass >> c.chp >> c.mchp >> c.catt >> c.attlvlups >> c.regen >> c.cxp >> c.lvl >> c.score >> c.dif;

	for (int i = 0; i < 5; i++)
	{
		weaponsarray[i] = { "", 0 };
		armorsarray[i] = { "", 0, 0, 0 };
		helmetsarray[i] = { "", 0, 0, 0 };
	}

	string w1, w2, w3; //loads an inventory item type
	string line1;
	ifstream sdata1;
	sdata1.open("char/weapons.txt");
	int counter1 = 0;
	getline(sdata1, line1);
	istringstream iss1(line1);
	iss1 >> c.we.wcatt >> w1 >> w2 >> w3;

	c.we.wname = w1 + " " + w2 + " " + w3;
	while (getline(sdata1, line1)) 
	{
		w1 = w2 = w3 = "";
		istringstream ssi(line1);
		ssi >> weaponsarray[counter1].wcatt >> w1 >> w2 >> w3;
		weaponsarray[counter1].wname = w1 + " " + w2 + " " + w3;
		counter1++;
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
		a1 = a2 = a3 = "";
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
		h1 = h2 = h3 = "";
		istringstream ss3(line3);
		ss3 >> helmetsarray[counter3].htotalstat >> helmetsarray[counter3].hregen >> helmetsarray[counter3].hmchp >> h1 >> h2 >> h3;
		helmetsarray[counter3].hname = h1 + " " + h2 + " " + h3;
		counter3++;
	}
	sdata3.close();

	system("CLS");
}

static void settings()
{
	mciSendString(L"open audio/sfx/elitesfx.mp3 type mpegvideo alias elitesfx", NULL, 0, NULL); // to hear sfx when selecting its vol
	int cs = 0;
	int mc = 0;
	bool vi = true;
	bool loop;
	do
	{
		system("CLS");
		cout << "Settings" << endl;
		cout << "1 - New Character Difficulty\n2 - Toggle Full Screen launch\n3 - Music Volume\n4 - Sound Effects Volume\n5 - Change soundtrack (relaunch game).\n6 - toggle dungeon generation\n7 - exit" << endl;

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
			rpglib::volumeupdate(mvolume, sfxvolume);
			break;
		case 4: mciSendString(L"seek elitesfx to start", NULL, 0, NULL);
			mciSendStringA("play elitesfx", NULL, 0, NULL);
			cout << "Enter a sound effects volume level 0 - 10" << endl;
			cin >> sfxvolume;
			sfxvolume *= 100;
			rpglib::volumeupdate(mvolume, sfxvolume);
			break;
		case 5: cout << "Judah - 1\nAlina - 2" << endl;
			cin >> soundtracknum;
			break;
		case 6: cout << "On - 1\nOff - 0" << endl;
			cin >> dungeontype;
			if (dungeontype != 1 && dungeontype != 0)
				dungeontype = 1;
		case 7: vi = true;
			break;
		default: vi = false;
			break;
		}
	} while (vi != true);
	system("CLS");

	FILE* ofp = fopen("settings.txt", "w"); //update settings file
	fprintf(ofp, "FullScreen %d\nStart Difficulty %d\nMusic Volume %d\nEffects Volume %d\nSoundtrack %d\nDungeon Generation %d", FullScreen, difsel, mvolume, sfxvolume, soundtracknum, dungeontype);
	fclose(ofp);
	mciSendString(L"close elitesfx", NULL, 0, NULL);
}

int main()
{
	FILE* ifp = fopen("settings.txt", "r"); //read in settings
	fscanf(ifp, "FullScreen %d\n", &FullScreen);
	fscanf(ifp, "Start Difficulty %d\n", &difsel);
	fscanf(ifp, "Music Volume %d\n", &mvolume);
	fscanf(ifp, "Effects Volume %d\n", &sfxvolume);
	fscanf(ifp, "Soundtrack %d\n", &soundtracknum);
	fscanf(ifp, "Dungeon Generation %d", &dungeontype);
	string st;
	if (soundtracknum == 2)
		st = "st2";
	else
		st = "st1";
	fclose(ifp);

	string em3 = "open audio/" + st + "/em3.mp3 type mpegvideo alias em3"; //creating all needed audio players
	string fighttheme = "open audio/" + st + "/fighttheme.mp3 type mpegvideo alias fighttheme";
	string fighttheme2 = "open audio/" + st + "/fighttheme2.mp3 type mpegvideo alias fighttheme2";
	string maintheme = "open audio/" + st + "/maintheme.mp3 type mpegvideo alias maintheme";
	string titletheme = "open audio/" + st + "/titletheme.mp3 type mpegvideo alias titletheme";
	string practicetheme = "open audio/" + st + "/practicetheme.mp3 type mpegvideo alias practicetheme";
	mciSendStringA(em3.c_str(), NULL, 0, NULL);
	mciSendStringA(fighttheme.c_str(), NULL, 0, NULL);
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
	int dungeonresult = 0;
	bool resetmenu = true;
	srand((unsigned)time(0));
	int ts;
	
	rpglib::volumeupdate(mvolume, sfxvolume);

	mciSendString(L"play titletheme repeat", NULL, 0, NULL);
	int pvpfullexit = titlescreen(); // returns 1 if pvp() wants to close app //handles screensize
	mciSendString(L"pause titletheme", NULL, 0, NULL);
	mciSendString(L"close titletheme", NULL, 0, NULL);
	mciSendString(L"seek maintheme to start", NULL, 0, NULL);
	mciSendString(L"play maintheme repeat", NULL, 0, NULL);
	system("CLS");

	if (pvpfullexit == 1) // app closed from pvp()
	{
		mciSendString(L"pause maintheme", NULL, 0, NULL);
		mciSendString(L"close maintheme", NULL, 0, NULL);
		mciSendString(L"close em3", NULL, 0, NULL);
		mciSendString(L"close fighttheme", NULL, 0, NULL);
		mciSendString(L"close fighttheme2", NULL, 0, NULL);
		mciSendString(L"close practicetheme", NULL, 0, NULL);
		mciSendString(L"close critsfx", NULL, 0, NULL);
		mciSendString(L"close itemdsfx", NULL, 0, NULL);
		mciSendString(L"close elitesfx", NULL, 0, NULL);
		mciSendString(L"close vsfx", NULL, 0, NULL);
		mciSendString(L"close dsfx", NULL, 0, NULL);
		mciSendString(L"close lvlupsfx", NULL, 0, NULL);
		return 0;
	}
	
	do
	{
		
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
				  c.attlvlups = 1;
				  c.regen = 1;
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
				  c.catt = 15;
				  c.attlvlups = 3;
				  c.regen = 1;
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
				  c.catt = 20;
				  c.attlvlups = 5;
				  c.regen = 1;
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
		}
		else if (temp2 == 2)
		{
			if (c.chp <= 0)
				cout << "No game to load.";
			else
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
			mciSendString(L"play maintheme repeat", NULL, 0, NULL);
		}
		else if (temp2 == 4)
		{
			mciSendString(L"pause maintheme", NULL, 0, NULL); // basically a copy of play()
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
			mciSendString(L"play maintheme repeat", NULL, 0, NULL);
		}
		else if (temp2 == 6)
		{
			settings();
			keepplaying = false;
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
			cin.clear(); // to protect an invalid input from causing an infinite loop
			if (c.cxp >= (int)xpcostscaler())
				lvlrdy = true;
			if (lvlrdy == true)
			{
				cout << "Your character has a pending level up\n0 - Level up screen" << endl;
			}

			cout << "1 - Battle\n2 - View character\n3 - Save game\n4 - Main Menu\n5 - Dungeon Map\n6 - Exit game" << endl;
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
			case 5: system("CLS");
				int dungeonresult = 0;
				char dun = 'A'; //select a dungeon and set difficulty and get file path for it

				{ //switch control didn't like these variable declarations, so processing at a different scope
				string dungeonmaprow;
				ifstream din("maps/dungeonsmap.txt");
				while (getline(din, dungeonmaprow))
					cout << dungeonmaprow << endl;
				cout << "Choose a dungeon or exit by entering 0" << endl;
				cin >> dun;
				}

				if (dun == '0')
				{
					system("CLS");
					break;
				}

				switch (dun)
				{
				case 'A':
					dungeonresult = dungeon(5, dungeontype, c, weaponsarray, armorsarray, helmetsarray);//return 0 if dungeon was exited, return 1 if player died					
					break;
				case 'B':
					dungeonresult = dungeon(10, dungeontype, c, weaponsarray, armorsarray, helmetsarray);//return 0 if dungeon was exited, return 1 if player died				
					break;
				case 'C':
					dungeonresult = dungeon(15, dungeontype, c, weaponsarray, armorsarray, helmetsarray);//return 0 if dungeon was exited, return 1 if player died				
					break;
				case 'D':
					dungeonresult = dungeon(20, dungeontype, c, weaponsarray, armorsarray, helmetsarray);//return 0 if dungeon was exited, return 1 if player died				
					break;
				case 'E':
					dungeonresult = dungeon(25, dungeontype, c, weaponsarray, armorsarray, helmetsarray);//return 0 if dungeon was exited, return 1 if player died				
					break;
				case 'F':
					dungeonresult = dungeon(30, dungeontype, c, weaponsarray, armorsarray, helmetsarray);//return 0 if dungeon was exited, return 1 if player died				
					break;
				case 'G':
					dungeonresult = dungeon(35, dungeontype, c, weaponsarray, armorsarray, helmetsarray);//return 0 if dungeon was exited, return 1 if player died			
					break;
				case 'H':
					dungeonresult = dungeon(40, dungeontype, c, weaponsarray, armorsarray, helmetsarray);//return 0 if dungeon was exited, return 1 if player died				
					break;
				case 'T':
					dungeonresult = TLE(c.lvl, c, weaponsarray, armorsarray, helmetsarray);//return 0 if dungeon was exited, return 1 if player died
					break;
				default:
					break;
				}
				if (dungeonresult == 1)
					killchar();
				break;
			case 6: keepplaying = false; // quite just gameplay loop
				resetmenu = false; // quite main menu loop
				break;
			default:
				break;
			}
		}
		keepplaying = true;	
	} while (resetmenu == true);
	mciSendString(L"pause maintheme", NULL, 0, NULL);
	mciSendString(L"close maintheme", NULL, 0, NULL);
	mciSendString(L"close em3", NULL, 0, NULL);
	mciSendString(L"close fighttheme", NULL, 0, NULL);
	mciSendString(L"close fighttheme2", NULL, 0, NULL);
	mciSendString(L"close practicetheme", NULL, 0, NULL);
	mciSendString(L"close critsfx", NULL, 0, NULL);
	mciSendString(L"close itemdsfx", NULL, 0, NULL);
	mciSendString(L"close elitesfx", NULL, 0, NULL);
	mciSendString(L"close vsfx", NULL, 0, NULL);
	mciSendString(L"close dsfx", NULL, 0, NULL);
	mciSendString(L"close lvlupsfx", NULL, 0, NULL);
	return 0;
}