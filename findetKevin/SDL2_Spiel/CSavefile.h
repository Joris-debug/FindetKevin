#pragma once
#include <iostream>
#include <ctime>
using namespace std;
class CSavefile
{
private:
	string playername;
	int levelNmbr;
	int currentHealth;
	int gameDifficulty;
	double totalScore;
	time_t creationDate;
public:
	~CSavefile();
	CSavefile(string playername, int gameDifficulty, time_t creationDate);
};

