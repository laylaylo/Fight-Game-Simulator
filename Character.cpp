#include "Character.h"
#include <string.h>
#include <stdio.h>
#include <algorithm>

using namespace std;

Character::Character(string _name, string _type, int _attack, int _defense, int _remainingHealth, int _nMaxRounds) {

	this->name = _name;
	this->type = _type;
	this->attack = _attack;
	this->defense = _defense;
	this->remainingHealth = _remainingHealth;
	this->nMaxRounds = _nMaxRounds;
	this->isAlive = true;
	this->nRoundsSinceSpecial = 0;
	this->healthHistory = new int[_nMaxRounds +1];
	this->healthHistory[0] = _remainingHealth;
}

Character::Character(const Character& character) {

	this->name = character.name;
	this->type = character.type;
	this->attack = character.attack;
	this->defense = character.defense;
	this->remainingHealth = character.remainingHealth;
	this->nMaxRounds = character.nMaxRounds;
	this->isAlive = character.isAlive;
	this->nRoundsSinceSpecial = character.nRoundsSinceSpecial;
	this->healthHistory = new int[nMaxRounds +1];
	this->healthHistory[0] = remainingHealth;
}

Character Character::operator=(const Character& character) {

	if(this == &character) {
		return *this;
	}

	// if this object was used before, free the memory it has used, test the effectiveness by assigning objects to one another sp many times! DOO
	if(this != NULL) {
		delete[] this->healthHistory;
		
	}

	this->name = character.name;
	this->type = character.type;
	this->attack = character.attack;
	this->defense = character.defense;
	this->remainingHealth = character.remainingHealth;
	this->nMaxRounds = character.nMaxRounds;
	this->isAlive = character.isAlive;
	this->nRoundsSinceSpecial = character.nRoundsSinceSpecial;
	this->healthHistory = character.healthHistory;	


	return *this;

}

bool Character::operator<(const Character& other) {

	int length = min(this->name.length(), other.name.length());
	int ind = 0;

	while(ind != length) {
		if(this->name[ind] > other.name[ind]) {
			return false;
			break;
		}else if(this->name[ind] < other.name[ind]) {
			return true;
			break;
		}else{
			ind++;
		}
	}
}

Character::~Character() {

	delete[] healthHistory;
	

}