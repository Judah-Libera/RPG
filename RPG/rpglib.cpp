#include "includes.h"

namespace rpglib
{
	void sortweapon(weapon arr[5])
	{
		for (int i = 0; i < 5 - 1; i++) // sort by descending power
			for (int j = 0; j < 5 - i - 1; j++)
				if (arr[j].wcatt < arr[j + 1].wcatt)
					swapweapon(arr[j], arr[j + 1]);
	}
	void sortarmor(armor arr[5])
	{
		for (int i = 0; i < 5 - 1; i++) // sort by descending power
			for (int j = 0; j < 5 - i - 1; j++)
				if (arr[j].atotalstat < arr[j + 1].atotalstat)
					swaparmor(arr[j], arr[j + 1]);
	}
	void sorthelmet(helmet arr[5])
	{
		for (int i = 0; i < 5 - 1; i++) // sort by descending power
			for (int j = 0; j < 5 - i - 1; j++)
				if (arr[j].htotalstat < arr[j + 1].htotalstat)
					swaphelmet(arr[j], arr[j + 1]);
	}

	void swapweapon(weapon& i, weapon& j)
	{
		weapon t = i;
		i = j;
		j = t;
	}
	void swaparmor(armor& i, armor& j)
	{
		armor t = i;
		i = j;
		j = t;
	}
	void swaphelmet(helmet& i, helmet& j)
	{
		helmet t = i;
		i = j;
		j = t;
	}

	void volumeupdate(int mvolume, int sfxvolume) //works in deadlib? pass mvolume and sfxvolume
	{
		std::string vol;
		vol = "setaudio em3 volume to ";
		vol.append(std::to_string(mvolume));
		mciSendStringA(vol.c_str(), NULL, 0, 0);
		vol = "setaudio fighttheme volume to ";
		vol.append(std::to_string(mvolume));
		mciSendStringA(vol.c_str(), NULL, 0, 0);
		vol = "setaudio fighttheme2 volume to ";
		vol.append(std::to_string(mvolume));
		mciSendStringA(vol.c_str(), NULL, 0, 0);
		vol = "setaudio maintheme volume to ";
		vol.append(std::to_string(mvolume));
		mciSendStringA(vol.c_str(), NULL, 0, 0);
		vol = "setaudio titletheme volume to ";
		vol.append(std::to_string(mvolume));
		mciSendStringA(vol.c_str(), NULL, 0, 0);
		vol = "setaudio practicetheme volume to ";
		vol.append(std::to_string(mvolume));
		mciSendStringA(vol.c_str(), NULL, 0, 0);
		vol = "setaudio critsfx volume to ";
		vol.append(std::to_string(sfxvolume));
		mciSendStringA(vol.c_str(), NULL, 0, NULL);
		vol = "setaudio itemdsfx volume to ";
		vol.append(std::to_string(sfxvolume));
		mciSendStringA(vol.c_str(), NULL, 0, NULL);
		vol = "setaudio elitesfx volume to ";
		vol.append(std::to_string(sfxvolume));
		mciSendStringA(vol.c_str(), NULL, 0, NULL);
		vol = "setaudio vssfx volume to ";
		vol.append(std::to_string(sfxvolume));
		mciSendStringA(vol.c_str(), NULL, 0, NULL);
		vol = "setaudio dssfx volume to ";
		vol.append(std::to_string(sfxvolume));
		mciSendStringA(vol.c_str(), NULL, 0, NULL);
		vol = "setaudio lvlupsfx volume to ";
		vol.append(std::to_string(sfxvolume));
		mciSendStringA(vol.c_str(), NULL, 0, NULL);
	}
}