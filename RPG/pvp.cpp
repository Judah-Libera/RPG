//unaltered since last backup

#pragma warning(disable: 4996)

#include "includes.h"

using namespace std;

static weapon weaponsarray[5];
static armor armorsarray[5];
static helmet helmetsarray[5];
static weapon weaponsarray2[5]; // second set will be needed as these are no logner a part of character
static armor armorsarray2[5];
static helmet helmetsarray2[5];
static character c1, c2;

static void pvpplay();
static character pvpheal(character c);
static void pvpviewchar(int pac, character c, weapon weaponsarray[], armor armorsarray[], helmet helmetsarray[]);
static character pvploadchar(int pnum);
static character pvpnewchar();
int pvp();

static void pvpplay()
{
	cout << "When you see 'ATTACK #' hit the number then enter before times up" << endl;

	int numcheck = 10;
	int damage = 0;
	int counter = 1;

	bool keepatt = true;
	while (keepatt == true)
	{
		srand((unsigned)time(0));
		int ct = ((rand() % 4) + 1) * 1000;
		int sucat = (rand() % 10);
		double atttime = 1;

		cout << "ROUND " << counter << endl;;
		cout << "P1 hp - " << c1.chp << "/" << c1.mchp << "\nP1 ai attack - " << c1.catt + c1.we.wcatt << "\n" << endl;
		cout << "\nP2 hp - " << c2.chp << "/" << c2.mchp << "\nP2 attack - " << c2.catt + c2.we.wcatt << "\n\n" << endl;

		//start p1 att
		cout << "Player 1 - ";
		system("pause");
		system("CLS");
		cout << "ATTACK: ";
		Sleep(ct);
		cout << sucat << endl;
		numcheck = 10;


		clock_t begin = clock();

		cin >> numcheck;

		clock_t end = clock();

		atttime = double(end - begin) / CLOCKS_PER_SEC;

		system("CLS");

		cout << "time to strike: " << atttime << endl;

		if (cin.fail() == true)
		{
			cin.clear();
			cin.ignore(std::numeric_limits<int>::max(), '\n');
			cout << "You entered a character." << numcheck << endl;
			numcheck = 10;
		}
		if (sucat == numcheck && atttime < 1)
		{
			int temp = c2.chp;
			c2.chp = c2.chp - 4 * (c1.catt + c1.we.wcatt) * (1 - atttime);
			temp = temp - c2.chp;
			cout << "HIT - " << temp << "\n" << endl;
			if (c2.chp < 0)
				c2.chp = 0;
		}
		else
		{
			if (sucat != numcheck)
				cout << "You entered " << numcheck << " instead of " << sucat << ".\n" << endl;
			if (atttime > 1)
				cout << "Not quite fast enough." << endl;
		}
		//end p1 att
		cout << "P1 hp - " << c1.chp << "/" << c1.mchp << "\nP1 ai attack - " << c1.catt + c1.we.wcatt << "\n" << endl;
		cout << "\nP2 hp - " << c2.chp << "/" << c2.mchp << "\nP2 attack - " << c2.catt + c2.we.wcatt << "\n\n" << endl;
		//start p2 att
		srand((unsigned)time(0));
		ct = ((rand() % 4) + 1) * 1000;
		sucat = (rand() % 10);
		atttime = 1;
		cout << "Player 2 - ";
		system("pause");
		system("CLS");
		cout << "ATTACK: ";
		Sleep(ct);
		cout << sucat << endl;
		numcheck = 10;

		begin = clock();

		cin >> numcheck;

		end = clock();

		atttime = double(end - begin) / CLOCKS_PER_SEC;

		system("CLS");

		cout << "time to strike: " << atttime << endl;

		if (cin.fail() == true)
		{
			cin.clear();
			cin.ignore(std::numeric_limits<int>::max(), '\n');
			cout << "You entered a character. " << numcheck << endl;
			numcheck = 10;
		}
		if (sucat == numcheck && atttime < 1)
		{
			int temp = c1.chp;
			c1.chp = c1.chp - 4 * (c2.catt + c2.we.wcatt) * (1 - atttime);
			temp = temp - c1.chp;
			cout << "HIT - " << temp << "\n" << endl;
			if (c1.chp < 0)
				c1.chp = 0;
		}
		else
		{
			if (sucat != numcheck)
				cout << "You entered " << numcheck << " instead of " << sucat << ".\n" << endl;
			if (atttime > 1)
				cout << "Not quite fast enough." << endl;
		}
		//end p2 att

		cout << "P1 hp - " << c1.chp << "/" << c1.mchp << "\nP1 ai attack - " << c1.catt + c1.we.wcatt << "\n" << endl;
		cout << "\nP2 hp - " << c2.chp << "/" << c2.mchp << "\nP2 attack - " << c2.catt + c2.we.wcatt << "\n\n" << endl;

		if (c1.chp == 0 || c2.chp == 0) // don't see p2's attack when this triggers
		{
			if (c2.chp < 1 && c1.chp >= 1)
				cout << "\nP1 WINS";
			else if (c1.chp < 1 && c2.chp >= 1)
				cout << "\nP2 WINS";
			else
				cout << "\nTIE";
			keepatt = false;
			cout << "on round " << counter << endl;
		}
		counter++;

		system("pause");
		system("CLS");
	}
}

static character pvpheal(character c)
{
	int ct = ((rand() % 4) + 1) * 1000;
	int numcheck;
	int sucheal = (rand() % 10);
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
		c.chp = c.chp + (((double)(c.regen + c.ae.aregen + c.he.hregen) * ((double)(c.mchp + c.ae.amchp + c.he.hmchp) / 3)) * (1 - atttime));
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
	return c;
}

static void pvpviewchar(int pac, character c, weapon weaponsarray[], armor armorsarray[], helmet helmetsarray[]) //pass in array for needed character
{
	cout << "Class - " << c.cclass << endl;
	cout << "HP - " << c.chp << "/" << c.mchp + c.ae.amchp + c.he.hmchp << endl;
	cout << "Regeneration - " << c.regen + c.ae.aregen + c.he.hregen << endl;
	cout << "attack - " << c.catt + c.we.wcatt << endl;
	cout << "Level - " << (int)c.lvl << "\n\n" << endl;
	cout << "\nCurrent Weapon: " << c.we.wname << "\nCurrent Armor: " << c.ae.aname << "\nCurrent Helmet: " << c.he.hname << endl;

	if (pac == 1)
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
					cout << i + 1 << " - " << weaponsarray[i].wname << " - " << weaponsarray[i].wcatt << " att" << endl;
				cout << "\nB - Armors in inventory." << endl;
				for (int i = 0; i < 5; i++)
					cout << i + 1 << " - " << armorsarray[i].aname << " - " << armorsarray[i].amchp << " hp" << " - " << armorsarray[i].aregen << " regen" << endl;
				cout << "\nC - Helmets in inventory." << endl;
				for (int i = 0; i < 5; i++)
					cout << i + 1 << " - " << helmetsarray[i].hname << " - " << helmetsarray[i].hmchp << " hp" << " - " << helmetsarray[i].hregen << " regen" << endl;

				cout << "\nTo switch items enter the letter of the item type you would like to replace, or D to delete/unequip. Enter 0 at any time to exit." << endl;
				cin >> mc;
				if (mc == 'a' || mc == 'A')
				{
					cout << "Enter the number of the weapon you would like to equip." << endl;
					cin >> mc2;
					if (mc2 == 1)
						c.we = weaponsarray[0];
					else if (mc2 == 2)
						c.we = weaponsarray[1];
					else if (mc2 == 3)
						c.we = weaponsarray[2];
					else if (mc2 == 4)
						c.we = weaponsarray[3];
					else if (mc2 == 5)
						c.we = weaponsarray[4];
					else
						skip = true;
				}
				else if (mc == 'b' || mc == 'B')
				{
					cout << "Enter the number of the armor you would like to equip." << endl;
					cin >> mc2;
					if (mc2 == 1)
						c.ae = armorsarray[0];
					else if (mc2 == 2)
						c.ae = armorsarray[1];
					else if (mc2 == 3)
						c.ae = armorsarray[2];
					else if (mc2 == 4)
						c.ae = armorsarray[3];
					else if (mc2 == 5)
						c.ae = armorsarray[4];
					else
						skip = true;
				}
				else if (mc == 'c' || mc == 'C')
				{
					cout << "Enter the number of the helmet you would like to equip." << endl;
					cin >> mc2;
					if (mc2 == 1)
						c.he = helmetsarray[0];
					else if (mc2 == 2)
						c.he = helmetsarray[1];
					else if (mc2 == 3)
						c.he = helmetsarray[2];
					else if (mc2 == 4)
						c.he = helmetsarray[3];
					else if (mc2 == 5)
						c.he = helmetsarray[4];
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
							weaponsarray[0] = { "",0 };
						else if (mc5 == 2)
							weaponsarray[0] = { "",0 };
						else if (mc5 == 3)
							weaponsarray[0] = { "",0 };
						else if (mc5 == 4)
							weaponsarray[0] = { "",0 };
						else if (mc5 == 5)
							weaponsarray[0] = { "",0 };
						else if (mc5 == 6)
						{
							for (int i = 0; i < 5 - 1; i++)
								for (int j = 0; j < 5 - i - 1; j++)
									if (weaponsarray[j].wcatt < weaponsarray[j + 1].wcatt)
										deadlibrary::swapweapon(weaponsarray[j], weaponsarray[j + 1]);
							weaponsarray[4] = c.we;
							c.we = { "",0 };
						}
					}
					else if (mc4 == 'b' || mc == 'B')
					{
						cout << "Enter the number of the armor you would like to delete or enter 6 to unequip your current armor." << endl;
						cin >> mc5;
						if (mc5 == 1)
							armorsarray[0] = { "",0 };
						else if (mc5 == 2)
							armorsarray[0] = { "",0 };
						else if (mc5 == 3)
							armorsarray[0] = { "",0 };
						else if (mc5 == 4)
							armorsarray[0] = { "",0 };
						else if (mc5 == 5)
							armorsarray[0] = { "",0 };
						else if (mc5 == 6)
						{
							for (int i = 0; i < 5 - 1; i++)
								for (int j = 0; j < 5 - i - 1; j++)
									if (armorsarray[j].atotalstat < armorsarray[j + 1].atotalstat)
										deadlibrary::swaparmor(armorsarray[j], armorsarray[j + 1]);
							armorsarray[4] = c.ae;
							c.ae = { "",0 };
						}
					}
					else if (mc4 == 'c' || mc == 'C')
					{
						cout << "Enter the number of the helmet you would like to delete or enter 6 to unequip your current helmet." << endl;
						cin >> mc5;
						if (mc5 == 1)
							helmetsarray[0] = { "",0 };
						else if (mc5 == 2)
							helmetsarray[0] = { "",0 };
						else if (mc5 == 3)
							helmetsarray[0] = { "",0 };
						else if (mc5 == 4)
							helmetsarray[0] = { "",0 };
						else if (mc5 == 5)
							helmetsarray[0] = { "",0 };
						else if (mc5 == 6)
						{
							for (int i = 0; i < 5 - 1; i++)
								for (int j = 0; j < 5 - i - 1; j++)
									if (helmetsarray[j].htotalstat < helmetsarray[j + 1].htotalstat)
										deadlibrary::swaphelmet(helmetsarray[j], helmetsarray[j + 1]);
							helmetsarray[4] = c.he;
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
			cout << "Level - " << c.lvl << "\n\n" << endl;
			cout << "\nCurrent Weapon: " << c.we.wname << "\nCurrent Armor: " << c.ae.aname << "\nCurrent Helmet: " << c.he.hname << endl;
			system("pause");
		}
	}
	system("CLS");
}

static character pvploadchar(int pnum) //redo completely
{
	character c; //load into this
	return c;
}

static character pvpnewchar()
{
	character c;
	system("CLS");
	cout << "What type of character you would like to make?\n1 - Orc - high health but low damage. Makes for a forgving combat experience.\n2 - Human - medium health and medium damage. A good balance in required combat ability. \n3 - Elf - low health but high damage. Requires the ability to consistently win rounds." << endl;
	int temp;
	cin >> temp;
	system("CLS");
	switch (temp) //character create, recopy
	{
	default:
		break;
	}
	return c;
}

int pvp()
{
	mciSendString(L"open fighttheme.mp3 type mpegvideo alias fighttheme", NULL, 0, NULL);
	mciSendString(L"open fighttheme2.mp3 type mpegvideo alias fighttheme2", NULL, 0, NULL);
	mciSendString(L"open titletheme.mp3 type mpegvideo alias titletheme", NULL, 0, NULL);
	bool rematch = true;
	bool newchars = true;
	bool battleloop = true;
	bool goodinput = false;
	int ts, mmc;
	while (rematch == true)
	{
		if (newchars == true)
		{
			c1 = pvploadchar(1);
			c2 = pvploadchar(2);
		}
		while (battleloop == true)
		{
			cout << "0 - Battle\n1 - P1 View character\n2 - P2 View character" << endl;
			cin >> mmc;
			system("CLS");
			switch (mmc)
			{
			case 0: ts = rand() % 2;
				if (ts == 0)
				{
					battleloop = false;
					mciSendString(L"pause titletheme", NULL, 0, NULL);
					mciSendString(L"play fighttheme repeat", NULL, 0, NULL);
					pvpplay();
					mciSendString(L"pause fighttheme", NULL, 0, NULL);
					mciSendString(L"play titletheme repeat", NULL, 0, NULL);
				}
				else
				{
					battleloop = false;
					mciSendString(L"pause titletheme", NULL, 0, NULL);
					mciSendString(L"play fighttheme2 repeat", NULL, 0, NULL);
					pvpplay();
					mciSendString(L"pause fighttheme2", NULL, 0, NULL);
					mciSendString(L"play titletheme repeat", NULL, 0, NULL);
				}
				break;
			case 1: pvpviewchar(1, c1, weaponsarray, armorsarray, helmetsarray);
				break;
			case 2: pvpviewchar(1, c2, weaponsarray2, armorsarray2, helmetsarray2);
				break;
			default:
				break;
			}
		}
		battleloop = true;
		while (goodinput == false)
		{
			cout << "0 - rematch\n1 - select new characters\n2 - Main Menu\n3 - Exit game" << endl;
			cin >> mmc;
			system("CLS");
			switch (mmc)
			{
			case 0: rematch = true;
				newchars = false;
				goodinput = true;
				break;
			case 1: newchars = true;
				rematch = true;
				goodinput = true;
				break;
			case 2: rematch = false;
				goodinput = true;
				return 0;
				break;
			case 3: rematch = false;
				goodinput = true;
				return 1;
				break;
			default:
				goodinput = false;
				break;
			}
		}
	}
	mciSendString(L"pause maintheme", NULL, 0, NULL);
	return 0;
}