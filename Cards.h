#include <vector>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <time.h>


using namespace std;

class Player;

class Card {
public:
	Card();
	Card(string unitType, string country);
	Card(const Card& cardToCopy);

	string toString();

	virtual Card& operator=(const Card& c);
	friend ostream& operator<<(ostream& out, const Card& c);
	string unitType;
	string country;
};


