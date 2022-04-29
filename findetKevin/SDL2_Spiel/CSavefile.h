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
	char creationDate[26];
public:
	~CSavefile();
	CSavefile(string playername, int gameDifficulty);
};

