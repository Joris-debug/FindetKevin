#include "CSavefile.h"

CSavefile::~CSavefile()
{
}

CSavefile::CSavefile(string playername, int gameDifficulty, time_t creationDate)
{
	this->playername = playername;
	this->levelNmbr = 1;
	this->currentHealth = 5;
	this->gameDifficulty = gameDifficulty;
	this->totalScore = 0;
	this->creationDate = creationDate;
}
