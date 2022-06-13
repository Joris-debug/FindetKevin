#include "CSavefile.h"
#include <stddef.h>
#include <cstdlib>
#include <string>
struct fileData		//Mehr brauche ich nicht in der externen Datei abzuspeichern 
{
	char playername[11];
	int levelNmbr;
	int currentHealth;
	int gameDifficulty;
	double totalScore;
	char creationDate[26];
};

CSavefile::~CSavefile()
{
}

CSavefile::CSavefile()
{
	this->playername = "Bob";
	this->levelNmbr = 1;
	this->currentHealth = 5;
	this->gameDifficulty = 1;
	this->totalScore = 0;
	time_t result = time(NULL);
	ctime_s(creationDate, 26, &result);
	cout << creationDate << endl;
	nextFile = nullptr;
}

CSavefile::CSavefile(string playername, int gameDifficulty)
{
	this->playername = playername;
	this->levelNmbr = 1;
	this->currentHealth = 5;
	this->gameDifficulty = gameDifficulty;
	this->totalScore = 0;
	time_t result = time(NULL);
	ctime_s(creationDate, 26, &result);
	cout << creationDate;
	nextFile = nullptr;
	cout << playername << "'s, neue Savefile wurde erstellt" << endl;
}

CSavefile::CSavefile(string playername, int levelNmbr, int currentHealth, int gameDifficulty, double totalScore, char* creationDate, CSavefile* previous)
{
	this->playername = playername;
	this->levelNmbr = levelNmbr;
	this->currentHealth = currentHealth;
	this->gameDifficulty = gameDifficulty;
	this->totalScore = totalScore;
	strncpy_s(this->creationDate, creationDate, 26);
	cout << creationDate;
	nextFile = previous;
	cout << playername << "'s, Savefile wurde eingelesen" << endl;
}

void CSavefile::SchreibenDerSpeicherdaten()
{
	FILE* datei;
	if ((fopen_s(&datei, "Savefiles.kev", "wb")) != NULL)
	{
		return;
	}
	fileData tempSaveFile;
	CSavefile* next = this;

	while (next != nullptr)
	{
		fileData tempSaveFile
		{
			"NameTemp",
			next->levelNmbr,
			next->currentHealth,
			next->gameDifficulty,
			next->totalScore,
			"DatumTemp"
		};
		strncpy_s(tempSaveFile.playername, next->playername.c_str(), 11);	//Jetzt kommt der echte Name
		strncpy_s(tempSaveFile.creationDate, next->creationDate, 26);	//Jetzt kommt das echte Datum

		fwrite(&tempSaveFile, sizeof(struct fileData), 1, datei);
		next = next->nextFile;
	}
	fclose(datei);

	return;
}

CSavefile* CSavefile::EinlesenDerSpeicherdaten()
{
	FILE* datei;
	if ((fopen_s(&datei, "Savefiles.kev", "a+b")) != NULL)
	{
		return nullptr;
	}
	fileData tempSaveFile;
	CSavefile * previous = nullptr;
	while (fread(&tempSaveFile, sizeof(struct fileData), 1, datei) == 1)
	{
		previous = new CSavefile(tempSaveFile.playername, tempSaveFile.levelNmbr, tempSaveFile.currentHealth, tempSaveFile.gameDifficulty, tempSaveFile.totalScore, tempSaveFile.creationDate, previous);
	}
	fclose(datei);
	return previous;
}
