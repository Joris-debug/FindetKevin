#pragma once
#include <iostream>
#include <ctime>
using namespace std;
class CSavefile
{
private:
	CSavefile* nextFile;
	string playername;
	int levelNmbr;
	int currentHealth;
	int gameDifficulty;
	unsigned long int totalScore;
	char creationDate[26];
public:
	~CSavefile();
	CSavefile();
	CSavefile(string playername, int gameDifficulty);	//Für das erstmalige anlegen eines Speicherstandes
	CSavefile(string playername, int levelNmbr,	int currentHealth, int gameDifficulty, double totalScore, char* creationDate, CSavefile* previous);	//Für das wiederanlegen eines Speicherstandes
	void SchreibenDerSpeicherdaten(); 
	inline char* getCreationDate() { return creationDate; }
	inline int* getLevel() { return &levelNmbr; }
	inline unsigned long int* getTotalScore() { return &totalScore; }
	inline int getDifficulty() { return gameDifficulty; }
	inline string getDifficultyString() { if (this->gameDifficulty == 1) return "Schw"; if (this->gameDifficulty == 2) return "Mitt"; else return "Einf"; }
	inline string getPlayername() { return playername; }
	inline CSavefile* getNextFile() { return nextFile; }
	inline void setNextFile(CSavefile* nextFile) { this->nextFile = nextFile; }
	static CSavefile* EinlesenDerSpeicherdaten();
};

