#include <iostream>
#include <ostream>
#include <vector>
#include <algorithm>

class Card;
class Player;
class Territory;

class Player
{

	public:
		vector<Territory*> owned;
		vector<Card*> hand;
		int numArmies;
		string name;
		
		Player();
		Player(string name);
		Player(Player &plr);
		~Player();
        
		vector <Territory*> toAttack();
		vector <Territory*> toDefend();
		void attack(Territory* t1, Territory* t2);
        
		friend std::ostream& operator<<(std::ostream & stream, const Player & plr);
		void operator=(Player & plr);

};

