//unaltered since last backup

#include "includes.h"

using namespace std;

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
	weapon we = { "", 0 };
	weapon weaponsarray[5];
	armor ae = { "", 0, 0 };
	armor armorsarray[5];
	helmet he = { "", 0, 0 };
	helmet helmetsarray[5];
};
character c1, c2;

void pvpplay();
character pvpheal(character c);
void pvpviewchar(int pac, character c);
character pvploadchar(int pnum);
character pvpnewchar();

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
			case 1: pvpviewchar(1, c1);
				break;
			case 2: pvpviewchar(1, c2);
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

void pvpplay()
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

character pvpheal(character c)
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

void pvpviewchar(int pac, character c)
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
					cout << i + 1 << " - " << c.weaponsarray[i].wname << " - " << c.weaponsarray[i].wcatt << " att" << endl;
				cout << "\nB - Armors in inventory." << endl;
				for (int i = 0; i < 5; i++)
					cout << i + 1 << " - " << c.armorsarray[i].aname << " - " << c.armorsarray[i].amchp << " hp" << " - " << c.armorsarray[i].aregen << " regen" << endl;
				cout << "\nC - Helmets in inventory." << endl;
				for (int i = 0; i < 5; i++)
					cout << i + 1 << " - " << c.helmetsarray[i].hname << " - " << c.helmetsarray[i].hmchp << " hp" << " - " << c.helmetsarray[i].hregen << " regen" << endl;

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
			cout << "Level - " << c.lvl << "\n\n" << endl;
			cout << "\nCurrent Weapon: " << c.we.wname << "\nCurrent Armor: " << c.ae.aname << "\nCurrent Helmet: " << c.he.hname << endl;
			system("pause");
		}
	}
	system("CLS");
}

character pvploadchar(int pnum)
{
	character c;
	character savedgames[10];
	FILE* pvpifpb = fopen("chardata.bin", "rb");
	fread(&savedgames, sizeof(character) * 10, 1, pvpifpb);
	fclose(pvpifpb);

	system("CLS");
	int cs = 11;
	bool vi = true;
	cout << "Player " << pnum << ", Please select the save slot you would like to load. Hp will be filled for the duel." << endl;
	cout << "\n0 - new character" << endl;
	for (int i = 0; i < 10; i++)
		cout << i+1 << " - " << savedgames[i].cclass << " level " << savedgames[i].lvl << endl;
	do
	{
		vi = true;
		cin >> cs;
		switch (cs)
		{
		case 0: c = pvpnewchar();
			break;
		case 1: c = savedgames[0];
			break;
		case 2: c = savedgames[1];
			break;
		case 3: c = savedgames[2];
			break;
		case 4: c = savedgames[3];
			break;
		case 5: c = savedgames[4];
			break;
		case 6: c = savedgames[5];
			break;
		case 7: c = savedgames[6];
			break;
		case 8: c = savedgames[7];
			break;
		case 9: c = savedgames[8];
			break;
		case 10: c = savedgames[9];
			break;
		case 11:
			break;
		default: vi = false;
			break;
		}
	} while (vi != true);
	system("CLS");
	c.chp = c.mchp;
	return c;
}

character pvpnewchar()
{
	character c;
	system("CLS");
	cout << "What type of character you would like to make?\n1 - Orc - high health but low damage. Makes for a forgving combat experience.\n2 - Human - medium health and medium damage. A good balance in required combat ability. \n3 - Elf - low health but high damage. Requires the ability to consistently win rounds." << endl;
	int temp;
	cin >> temp;
	system("CLS");
	switch (temp)
	{
	case 1: c = { "Orc", 200, 200, 10, 0, 3, 0, 1, 0 };
		  break;
	case 2: c = { "Human", 100, 100, 20, 0, 3, 0, 1, 0 };
		  break;
	case 3: c = { "Elf", 50, 50, 40, 0, 3, 0, 1, 0 };
		  break;
	default:
		break;
	}
	return c;
}