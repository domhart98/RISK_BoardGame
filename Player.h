#include <iostream>
#include <ostream>
#include <vector>
#include <algorithm>

class OrdersList;
class Card;
class Player;
class Order;
class Territory;

class Player
{
	
	// Components of the Player object: owned territories, a hand of cards, and a list of (non-card) orders

	public:
		vector<Territory*> owned;
		vector<Card*> hand;
		int numArmies;
		string name;
		
		// The Player API: default constructor, copy constructor, parameterized constructor and destructor
  Player();
  Player(string name);
  Player(Player &plr);
  ~Player();
        // Lists of territories to attack or defend as derived from the Player's list of orders and cards
  vector <Territory*> toAttack();
  vector <Territory*> toDefend();
  void attack(Territory* t1, Territory* t2);
        // Adds an order to the Player's OrderList
  void issueOrder(Order* ord);
       // Prints out a listing of the player's components
  friend std::ostream& operator<<(std::ostream & stream, const Player & plr);
       // Assigns components to another Player object.
  void operator=(Player & plr);
};

