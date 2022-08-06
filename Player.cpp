
#include "Map.h"
#include "Cards.h"
#include "Player.h"


// COMP 345
// Player class implementation by Shane Fournier, February 13 2022.


	
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
		Player::Player(Player &plr)
		{
			owned = plr.owned;
			hand = plr.hand;
			numArmies = plr.numArmies;

		}
		// destructor
		Player::~Player()
		{
			for (Territory* owned : owned) {
				delete owned;
			}
		       
			for (Card* card : this->hand) {
				delete card;
			}

            owned.clear();
			hand.clear();
			
		}
		
		vector<Territory*> Player::toAttack()
		// Returns adjacent territories, that are not already owned by the player.
		// Iterates through each owned territory as listed, checks adjacent territories for ownership, then adds if not owned by player.
		{
			vector <Territory*> t;
		    for(Territory* terr : this->owned){
		    	
		    	for (Territory* adjTerr: terr->adjacentTerritories)
		    	{
		   
		    	    if (terr->name.compare(adjTerr->name) != 0)
		    	    {
						t.push_back(adjTerr);
		    	    }   
		    		 
				}
		    	
			}
			sort(t.begin(), t.end());
			vector<Territory*>::iterator lastUniqueTerritory;
			lastUniqueTerritory = unique(t.begin(), t.end());
			t.erase(lastUniqueTerritory, t.end());

			cout << "Adjacent Territories: " << endl;
			for (Territory* terr : t) {
				cout << terr->name << " " << terr->armyNumber << ", ";
			}
			cout << endl;
			return t;
		}
		
		vector<Territory*> Player::toDefend()
		// Prints and then returns owned territories
		{
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

	    std::ostream& operator<<(std::ostream& out, const Player& plr)
	   // Iterates through owned territories, and lists cards and orders.
		{
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
		
		void Player::operator=(Player & plr)
		// assignment operator.
		{
			owned = plr.owned;
			hand = plr.hand;
			
		}
		



