#include "Cards.h"
#include "Player.h"



//Card class
Card::Card() {
	this->unitType = "";
	this->country = "";
}
Card::Card(string unitType, string country) {
	this->unitType = unitType;
	this->country = country;
}
Card::Card(const Card& cardToCopy) {
	this->unitType = (cardToCopy.unitType);
	this->country = cardToCopy.unitType;
}

string Card::toString() {
	return "Country: " + this->country + "Unit Type: " + this->unitType;
}

Card& Card::operator=(const Card& cardToAssign) {
	this->unitType = (cardToAssign.unitType);
	return *this;
}
ostream& operator<<(ostream& out, const Card& cardToStream) {
	out << "Country: " << cardToStream.country << "Unit Type: " << cardToStream.unitType;
	return out;
}

