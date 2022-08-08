#include "Map.h"
#include "Cards.h"
#include "Player.h"

// default constructor
Player::Player(){
		  
	owned;
	hand;
	numArmies = 0;
	name = "";

}

// parameterized constructor
Player::Player(string name){
		  
	this->name = name;
	owned;
	hand;
	numArmies = 0;

}

// copy constructor
Player::Player(Player &plr){

	owned = plr.owned;
	hand = plr.hand;
	numArmies = plr.numArmies;

}

// destructor
Player::~Player(){

	for (Territory* owned : owned) {
		delete owned;
	}
		       
	for (Card* card : this->hand) {
		delete card;
	}

    owned.clear();
	hand.clear();
			
}
		
// Returns adjacent territories, that are not already owned by the player.
// Iterates through each owned territory as listed, checks adjacent territories for ownership, then adds if not owned by player.
vector<Territory*> Player::toAttack(){
			
	vector <Territory*> t;
	//Add all adjacent territories to vector
	for(Territory* terr : this->owned){
		    	
		for (Territory* adjTerr: terr->adjacentTerritories) 
		{
		   
		    t.push_back(adjTerr);   
		    		 
		}
		    	
	}

	//removes all owned territories from adjacent territories vector
	for (Territory* terr : this->owned) {

		for (Territory* adjTerr : t)
		{
			if(terr->name == adjTerr->name)
			t.push_back(adjTerr);

		}

	}

	//remove duplicate territories from adjacent territories vector
	sort(t.begin(), t.end());
	vector<Territory*>::iterator lastUniqueTerritory;
	lastUniqueTerritory = unique(t.begin(), t.end());
	t.erase(lastUniqueTerritory, t.end());

	//Display adjacent territories
	cout << "Adjacent Territories: " << endl;
	for (Territory* terr : t) {
		cout << terr->name << " Defending Armies: " << terr->armyNumber << " || ";
	}
	cout << endl;
	return t;

}
	
// Prints and then returns owned territories
vector<Territory*> Player::toDefend(){

	cout << "Owned Territories: " << endl;
		for (Territory* terr : this->owned) {
			cout << terr->name << ", ";
		}
	cout << endl;
	return owned;

}

//t1 is the attacking territory, t2 is the defending territory. Roll to resolve, based on number of armies used.
void Player::attack(Territory* t1, Territory* t2) {
			
	if (t1->armyNumber < 2) {
		cout << "Insufficient armies to attack from this territory." << endl;
		return;
	}
	cout << "How many armies to attack with(1~3)?: ";
	int attackingArmies;
	cin >> attackingArmies;

	cout << "How many armies to defend with(1~2)?: ";
	int defendingArmies;
	cin >> defendingArmies;

	//One army must remain on the attacking territory. 
	if (attackingArmies < t1->armyNumber && defendingArmies <= t2->armyNumber) {
		int attackRolls[3];
		int defendRolls[2];

		for (int i = 0; i < attackingArmies; i++) {
			attackRolls[i] = rand() % 6 + 1;
		}
		for (int i = 0; i < defendingArmies; i++) {
			defendRolls[i] = rand() % 6 + 1;
		}

		sort(attackRolls, attackRolls + 3, greater<int>());
		sort(defendRolls, defendRolls + 2, greater<int>());

		for (int i = 0; i < defendingArmies; i++) {
			if (attackRolls[i] > defendRolls[i]) {
				t2->armyNumber--;
				if (t2->armyNumber == 0) {
					t2->owner = t1->owner;
					t1->armyNumber--;
					t2->armyNumber++;
					return;
				}
			}
			else {
				t1->armyNumber--;
			}
		}
	}
	return;

}

// ostream overload
std::ostream& operator<<(std::ostream& out, const Player& plr){

	out << "Player name: " << plr.name << endl;
	
	out << "Owned territories: "; 
	for (Territory* terr : plr.owned) {
		out << *terr;
	}
			
	out << endl;

	out << "Cards in hand: ";
	for (Card* card : plr.hand) {
		out << *card;
	}

	out << endl;
			
	return out;

}

// assignment operator.
void Player::operator=(Player & plr){
	
	name = plr.name;
	owned = plr.owned;
	hand = plr.hand;
			
}


