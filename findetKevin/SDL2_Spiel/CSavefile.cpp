#include "CSavefile.h"

CSavefile::~CSavefile()
{
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
	cout << creationDate << endl;
	cout << playername << ", neue Savefile wurde erstellt" << endl;
}
