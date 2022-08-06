
#include <vector>
#include <iostream>
#include <list>
#include <unordered_set>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

class Player;

class Territory {
public:
    string name;
    int territoryNumber;
    int armyNumber;
    vector<Territory*> adjacentTerritories;
    Player* owner;

    Territory();
    Territory(string name, int territoryNum, int armyNum);
    Territory(const Territory& territory);
    Territory& operator=(const Territory& territory);
    friend ostream& operator<<(ostream& out, const Territory& territory);
    ~Territory();
};

class Continent {
public:
    std::string name;
    int bonusPts;
    vector<Territory*> territories;

    Continent();
    Continent(std::string name, int bonusPts);
    Continent(const Continent& continent);
    Continent& operator=(const Continent& continent);
    friend ostream& operator<<(ostream& out, const Continent& continent);
    ~Continent();
};

class Edge {
public:
    Territory* start;
    Territory* stop;

    Edge();
    Edge(Territory* start, Territory* stop);
    Edge(const Edge& edge);
    Edge& operator=(const Edge& edge);
    friend ostream& operator<<(ostream& out, const Edge& edge);
    ~Edge();
};

class Map {
public:
    vector<Territory*> territories;
    vector<Continent*> continents;
    vector<Edge*> edges;

    Map();
    Map(const Map& m);
    Map& operator=(const Map& m);
    friend ostream& operator<<(ostream& out, const Map& m);
    ~Map();

    bool isAdjacent(Territory* t1, Territory* t2);
    Territory* findTerritory(int territoryNum);
    void searchGraph(unordered_set<string>& names, const Territory* territory);
    void searchSubGraph(unordered_set<string>& nameset, const Continent* continent, const Territory* territory);
    bool validate();
};

class MapLoader {
public:
    Map* map;

    MapLoader();
    MapLoader(const MapLoader& ml);

    MapLoader& operator=(const MapLoader& ml);
    friend ostream& operator<<(ostream& out, const MapLoader& ml);
    void loadMap(const std::string& file);
};
