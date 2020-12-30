#include "Character.h"


#include <iostream>
#include <fstream>
#include "Character.h"
#include <vector>
#include <iterator>

using namespace std;

bool fight(Character& attacker, Character& defender, Character& hobbitAttacker, string isSpecial, int* numOfDeadsDefender, int* numOfDeadsAttacker, vector<Character*>* deadsDefender, vector<Character*>* deadsAttacker) { //no special yet

	bool isWarContinue = true;

	if(attacker.attack > defender.defense) {
		defender.remainingHealth -= (attacker.attack - defender.defense);
	}
	 
	if(defender.remainingHealth <= 0 ) {
		defender.remainingHealth = 0;
		defender.isAlive = false;  
		deadsDefender->push_back(&defender); 
		(*numOfDeadsDefender)++;
		if(defender.type == "Hobbit") {
			isWarContinue = false;
		}
		if(*numOfDeadsDefender == 4) {
			isWarContinue =false;
		}
	}
	
	if(isSpecial == "SPECIAL") {
		if(attacker.type == "Elves" && attacker.nRoundsSinceSpecial >= 10) {
			attacker.nRoundsSinceSpecial = -1;
			int halfOfHealth = attacker.remainingHealth / 2;
			attacker.remainingHealth /= 2;;
			hobbitAttacker.remainingHealth += halfOfHealth;
		}

		if(attacker.type == "Dwarfs" && attacker.nRoundsSinceSpecial >= 20){
			attacker.nRoundsSinceSpecial = -1;
			if(attacker.attack > defender.defense) {
				if((defender.remainingHealth -= (attacker.attack - defender.defense)) <= 0 ) {
					defender.remainingHealth = 0;
					defender.isAlive = false;
				}
			}
		}
		if(attacker.type == "Wizards" && attacker.nRoundsSinceSpecial >= 50){
			int n = deadsAttacker->size();
			if(n > 0) {
				attacker.nRoundsSinceSpecial = -1;
				if(!(*deadsAttacker)[n-1]->isAlive) {
					(*deadsAttacker)[n-1]->isAlive = true;
					(*deadsAttacker)[n-1]->remainingHealth = (*deadsAttacker)[n-1]->healthHistory[0];
					(*deadsAttacker)[n-1]->nRoundsSinceSpecial = 0;
					(*numOfDeadsAttacker)--;
				}
				if((*deadsAttacker)[n-1]->isAlive) {
					(*deadsAttacker)[n-1]->remainingHealth = (*deadsAttacker)[n-1]->healthHistory[0];
				}
			}
			 
		}
	}

	return isWarContinue;

}

Character* findAlive (vector<Character*> community, string name) { // replaces dead character by name-wise alphabetically next alive 
	int ind1;
	int ind2;
	for(int i = 0; i < 5; i++) {
		if(community[i]->name == name) {
			ind1 = i;
			ind2 = i;
			break;
		}
	}
	while(!community[ind1]->isAlive && ind1 < community.size()) {
		ind1++;
	}
	if(community[ind1]->isAlive) {
		return community[ind1];
	}
	while(!community[ind2]->isAlive && ind2 > 0) {
		ind2--;
	}
	if(community[ind2]->isAlive) {
		return community[ind2];
	}
}

vector<Character*> sort(vector<Character*> communityPtr) { // sorts vector name-wise alphabetically 

	Character* temp;

	for(int i = 0; i < 5; i++) {
		for(int j = i + 1; j < 5; j++) {
			Character a =(*communityPtr[i]);
			Character b =(*communityPtr[j]);
			if(!(a < (b))) {		
				temp = communityPtr[j];
				communityPtr[j] = communityPtr[i];
				communityPtr[i] = temp;
			}
		}		
	}
	return communityPtr;
}

int main(int argc, char* argv[]) {

	string infile_name = argv[1]; 
	string outfile_name = argv[2]; 

	ifstream infile;
	infile.open(infile_name);

	int maxNumOfRounds; 
	infile >> maxNumOfRounds;
	
	vector<Character> community1;

	for(int i = 0; i < 5; i++) { // takes data of community1 from the input file, creates characters and puts them into vector
		string name, type;
		int attack, defense, health;
		infile >> name >> type >> attack >> defense >> health;
		Character created = Character(name, type, attack, defense, health, maxNumOfRounds);
	    community1.push_back(created);
	}


	 vector<Character> community2;

	for(int i = 0; i < 5; i++) { // takes data of community2 from the input file, creates characters and puts them into vector
		string name, type;
		int attack, defense, health;
		infile >> name >> type >> attack >> defense >> health;
		Character created = Character(name, type, attack, defense, health, maxNumOfRounds);
		community2.push_back(created);
	}

	
	vector<Character*> community1Ptr;
	vector<Character*> community2Ptr;

	for(int i=0; i < 5; i++) { // creates pointer of vectors in order to make changes in methods
		community1Ptr.push_back(&community1[i]);
		community2Ptr.push_back(&community2[i]);
	}

	community1Ptr = sort(community1Ptr);
	community2Ptr = sort(community2Ptr);

	vector<Character*> deadsOfCom1;
	vector<Character*> deadsOfCom2;

	int numOfDeads1 = 0;
	int numOfDeads2 = 0;
	bool isWarContinue = true;
	int roundNum = 0;
	int winner;

	while(maxNumOfRounds > 0 && isWarContinue) { 
		string attackerName, defenderName, isSpecial;
		infile >> attackerName >> defenderName >> isSpecial;

		Character *attacker, *defender, *hobbitAttacker;

		bool comm1CheckAttacker = false;
		bool comm1CheckDefender = false;
		bool comm2CheckAttacker = false;
		bool comm2CheckDefender = false;

		roundNum++;

		for(int i=0; i < 5; i++) {
			if(community1Ptr[i]->name == attackerName) {
				attacker = community1Ptr[i];
				comm1CheckAttacker = true;
				break;
			}else if(community2Ptr[i]->name == attackerName) {
				attacker = community2Ptr[i];
				comm2CheckAttacker = true;
				break;
			}
		}

		for(int i=0; i < 5; i++) {
			if(community1Ptr[i]->name == defenderName) {
				comm1CheckDefender = true;
				defender = community1Ptr[i];
				break;
			}else if(community2Ptr[i]->name == defenderName) {
				comm2CheckDefender = true;
				defender = community2Ptr[i];
				break;
			}
		}

		for(int i = 0; i < 5; i++) {
			if(comm1CheckAttacker && community1Ptr[i]->type == "Hobbit") {
				hobbitAttacker = community1Ptr[i];
			}
			if(comm1CheckAttacker && community2Ptr[i]->type == "Hobbit") {
				hobbitAttacker = community2Ptr[i];
			}
		}

		if(!attacker->isAlive) {
			if(comm1CheckAttacker) {
				attacker = findAlive(community1Ptr, attacker->name);
			}
			if(comm2CheckAttacker) {
				attacker = findAlive(community2Ptr, attacker->name);
			}
		}

		if(!defender->isAlive) {
			if(comm1CheckDefender) {
				defender = findAlive(community1Ptr, defender->name);
			}
			if(comm2CheckDefender) {
				defender = findAlive(community2Ptr, defender->name);
			}
		}

		if((comm1CheckDefender && comm1CheckAttacker) || (comm2CheckDefender && comm2CheckAttacker)) { // attacker and defender should be in different communitites
			// do nothing
		}else{
			if(comm1CheckDefender) {
			isWarContinue = fight(*attacker, *defender, *hobbitAttacker, isSpecial, &numOfDeads1, &numOfDeads2, &deadsOfCom1, &deadsOfCom2);
			}else{
			isWarContinue = fight(*attacker, *defender, *hobbitAttacker, isSpecial, &numOfDeads2, &numOfDeads1, &deadsOfCom2, &deadsOfCom1);
			}
		}

		for(int i = 0; i < 5; i++) {
			community1Ptr[i]->healthHistory[roundNum] = community1Ptr[i]->remainingHealth; 
			community2Ptr[i]->healthHistory[roundNum] = community2Ptr[i]->remainingHealth;
		}

		for(int i =0 ; i < 5; i++) {
			community1Ptr[i]->nRoundsSinceSpecial++;
			community2Ptr[i]->nRoundsSinceSpecial++;
		}

		if(!isWarContinue) {

			if(comm1CheckAttacker) {
				winner = 1;
			}else{
				winner = 2;
			}
		}

		maxNumOfRounds--;

	}

	ofstream outfile;
	outfile.open(outfile_name);

	if(isWarContinue) {
		outfile << "Draw" << endl;
	}else{
		outfile << "Community-" << winner << endl;
	}

		outfile << roundNum << "\n" << (numOfDeads1 + numOfDeads2) << endl;

	for(int i = 0; i < 5; i++) {
		outfile << community1[i].name << " ";
		for(int j = 0; j < roundNum + 1; j++) {
			outfile << community1[i].healthHistory[j] << " " ;
		}
		outfile << "\n";
	}

	for(int i = 0; i < 5; i++) {
		outfile << community2[i].name << " ";
		for(int j = 0; j < roundNum + 1; j++) {
			outfile << community2[i].healthHistory[j] << " " ;
		}
		outfile << "\n";
	}	

	infile.close();
	outfile.close();

	community1Ptr.clear();
	community2Ptr.clear();
	deadsOfCom1.clear();
	deadsOfCom2.clear();

    return 0;
}