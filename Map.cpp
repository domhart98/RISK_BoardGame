#include "Map.h"
#include "Player.h"
#include "Cards.h"

//-------------------------------CONTINENT--------------------------------------------------
Continent::Continent() {
    this->name = "";
    this->bonusPts = 0;
}

Continent::Continent(string name, int bonusPts) {
    this->name = name;
    this->bonusPts = bonusPts;
}

Continent::Continent(const Continent& c) {
    name = c.name;
    this->bonusPts = c.bonusPts;
    for (Territory* t : c.territories) {
        this->territories.push_back(new Territory(*t));
    }
}

Continent& Continent::operator=(const Continent& c) {
    if (this == &c) {
        return *this;
    }

    this->name = c.name;
    this->bonusPts = c.bonusPts;
    this->territories.clear();
    for (Territory* t : c.territories) {
        this->territories.push_back(t);
    }
    return *this;
}

Continent::~Continent() {
}

ostream& operator<<(ostream& out, const Continent& c) {
    string territories;
    for (Territory* t : c.territories) {
        territories += " " + t->name;
    }
    out << "Continent Name: " << c.name << " | Bonus Points: " << c.bonusPts << " | Territories: [" << territories << "]";
    return out;
}

//-------------------------------TERRITORY--------------------------------------------------
Territory::Territory() {
    this->name = "";
    this->territoryNumber = 0;
    this->armyNumber = 0;
}
Territory::Territory(string name, int territoryNum, int armyNum) {
    this->name = name;
    this->territoryNumber = territoryNum;
    this->armyNumber = armyNum;
}

Territory::Territory(const Territory& t) {
    this->name = t.name;
    this->territoryNumber = t.territoryNumber;
    this->armyNumber = t.armyNumber;

    for (Territory* t : t.adjacentTerritories) {
        this->adjacentTerritories.push_back(new Territory(*t));
    }
    return;
}

Territory& Territory::operator=(const Territory& t) {
    if (this == &t) {
        return *this;
    }
    this->name = t.name;
    this->territoryNumber = t.territoryNumber;
    this->armyNumber = t.armyNumber;
    this->adjacentTerritories.clear();
    for (Territory* terr : t.adjacentTerritories) {
        this->adjacentTerritories.push_back(terr);
    }

    return *this;
}

Territory::~Territory() {

}

ostream& operator<<(ostream& out, const Territory& t) {
    out << "Territory Number : " << t.territoryNumber << " | Territory Name: " << t.name << " | Armies : " << t.armyNumber;
    return out;
}
//-------------------------------EDGE--------------------------------------------------
Edge::Edge() {
    this->start = nullptr;
    this->stop = nullptr;
}

Edge::Edge(Territory* start, Territory* stop) {
    this->start = start;
    this->stop = stop;
}

Edge::Edge(const Edge& edge) {
    // deep copy Territory objects
    this->start = new Territory(*edge.start);
    this->stop = new Territory(*edge.stop);
}

Edge& Edge::operator=(const Edge& edge) {
    if (this == &edge) {
        return *this;
    }
    
    // shallow copy Territory objects
    this->start = edge.start;
    this->stop = edge.stop;
    return *this;
}

ostream& operator<<(ostream& out, const Edge& edge) {
    out << "Edge: " << endl;
    out << "\tstart: " << "\t" << *edge.start << endl;
    out << "\tstop: " << "\t" << *edge.stop << endl;
    return out;
}

Edge::~Edge() {};



//-------------------------------MAP--------------------------------------------------
Map::Map() {


}

Map::~Map() {

    //Delete pointer values and free memory.
    for (Continent* c : this->continents) {
        delete c;
        c = nullptr;
    }
    for (Territory* t : this->territories) {
        delete t;
        t = nullptr;
    }
    for (Edge* e : this->edges) {
        delete e;
        e = nullptr;
    }
    continents.clear();
    territories.clear();
    edges.clear();
}

Map::Map(const Map& m) {
    for (Territory* t : m.territories) {
        this->territories.push_back(new Territory(*t));
    }
    for (Continent* c : m.continents) {
        this->continents.push_back(new Continent(*c));
    }
    for (Edge* e : m.edges) {
        this->edges.push_back(new Edge(*e));
    }
}

Map& Map::operator=(const Map& m) {
    if (this == &m) {
        return *this;
    }

    for (auto* territory : this->territories) {
        delete territory;
        territory = nullptr;
    }
    for (auto* continent : this->continents) {
        delete continent;
        continent = nullptr;
    }
    for (auto* edge : this->edges) {
        delete edge;
        edge = nullptr;
    }
    this->territories.clear();
    this->continents.clear();
    this->edges.clear();

    for (auto* continent : m.continents) {
        this->continents.push_back(continent);
    }
    for (auto* territory : m.territories) {
        this->territories.push_back(territory);
    }
    for (auto* edge : m.edges) {
        this->edges.push_back(edge);
    }

    return *this;
}

ostream& operator<<(ostream& out, const Map& m) {
    out << "[Continents]" << endl;
    for (Continent* c : m.continents) {
        out << *c << endl;
    }
    out << "[Territories]" << endl;
    for (Territory* t : m.territories) {
        out << *t << endl;
    }
    out << "[Borders]" << endl;
    for (Territory* t : m.territories) {
        out << t->territoryNumber;
        for (Territory* adjacentTerritory : t->adjacentTerritories) {
            out << " " << adjacentTerritory->territoryNumber;
        }
        out << endl;
    }
    return out;
}

//Takes 2 Territory objects as params. Returns true if t2 contained in t1 adjacent territories vector.
bool Map::isAdjacent(Territory* t1, Territory* t2) {
    for (Territory* terr : t1->adjacentTerritories) {
        if (terr->name == t2->name) {
            return true;
        }
    }

    return false;
}

Territory* Map::findTerritory(int territoryNum) {
    for (Territory* t : this->territories) {
        if (t->territoryNumber == territoryNum) {
            return t;
        }
    }
    return NULL;
}

bool Map::validate() {
    //Check if Map is connected. Start from a territory, then perform a depth first search on the graph.
    unordered_set<string> nameset;
    const Territory* startTerritory = territories[0];
    bool mapConnected = false;
    bool continentsConnected = false;
    bool territoryHasOneContinent = false;
    
    if (startTerritory == nullptr) {
        return false;
    }
    else {
        this->searchGraph(nameset, startTerritory);
    }
    //If every territory on the Map is reachable from the starting territory, the Map is connected.
    if (nameset.size() == this->territories.size()) {
        mapConnected = true;
    }
    else {
        mapConnected = false;
    }

    //Check if each territory belongs to only one continent. In this case, a territory can only be added to one Continent so we 
    //know that each territory belongs to one continent already.
    territoryHasOneContinent = true;

    
    //Check if each continent on the map is also connnected. For each continent, perform DFS.
    for (const Continent* c : this->continents) {
        const Territory* startTerritory = c->territories[0];
        nameset.clear();
        if (c->territories.size() == 0) {
            continentsConnected = false;
        }
        else {
            this->searchSubGraph(nameset, c, startTerritory);
        }
        
        if (nameset.size() != c->territories.size()) {
            continentsConnected = false;
        }
        else {
            continentsConnected = true;
        }
    }

    
    
    //If all conditions are true, return true. Otherwise, return false
    if (mapConnected && continentsConnected && territoryHasOneContinent) {
        cout << "Map is valid." << endl;
        return true;
    }
    else {
        cout << "Map is not valid." << endl;
        return false;
    }

}

//Recursive implementation of DFS algorithm. 
void Map::searchGraph(unordered_set<string>& nameset, const Territory* territory) {
    if (nameset.find(territory->name) != nameset.end()) {
        return;
    }
    
    nameset.insert(territory->name);

    for (const Territory* adjacentTerritory : territory->adjacentTerritories) {
        searchGraph(nameset, adjacentTerritory);
    }
}
//Recursive implementation of DFS algorithm to traverse continents(sub graphs)
void Map::searchSubGraph(unordered_set<string>& nameset, const Continent* c, const Territory* territory) {

    if (nameset.find(territory->name) != nameset.end()) {
        return;
    }

    nameset.insert(territory->name);

    for (const Territory* adjacentTerritory : territory->adjacentTerritories) {
        if (std::count(c->territories.begin(), c->territories.end(), adjacentTerritory) == 1) {
            searchSubGraph(nameset, c, adjacentTerritory);
        }
    }
}


//-------------------------------MAPLOADER--------------------------------------------------
MapLoader::MapLoader() {
    this->map = new Map();
}

MapLoader::MapLoader(const MapLoader& ml) {
    this->map = new Map(*ml.map); //deep copy map 
}

MapLoader& MapLoader::operator=(const MapLoader& ml) {
    if (this == &ml) {
        return *this;
    }

    delete this->map;
    this->map = ml.map; //shallow copy map
    return *this;
}

ostream& operator<<(ostream& out, const MapLoader& ml) {
    out << "Assignment operator overload: MapLoader class." << endl;
    out << ml.map << endl;
    return out;
}

//Read file, parse the data line-by-line, then use data to create Map object.
void MapLoader::loadMap(const std::string& filename) {

    bool validated = false;
    ifstream infile;


    try {
        infile.open(filename);
        cout << "Map file " << filename << " was successfully opened" << endl;
    }
    catch (const ifstream::failure& e) {
        cout << "Error opening file" << endl;
        exit(0);
    }



    bool continentFlag = false;
    bool territoryFlag = false;
    bool edgeFlag = false;
    string line;
    vector<string> tokens;

    while (getline(infile, line)) {
        stringstream linestream(line);
        std::string tok;
        
        while (getline(linestream, tok, ' ')) {
            tokens.push_back(tok);
        }

        if (line == "[continents]") {
            continentFlag = true;
            territoryFlag = false;
            edgeFlag = false;
        }
        else if (line == "[countries]") {
            continentFlag = false;
            territoryFlag = true;
            edgeFlag = false;
        }
        else if (line == "[borders]") {
            continentFlag = false;
            territoryFlag = false;
            edgeFlag = true;
        }
        else if (continentFlag && tokens.size() != 0) {
            Continent* c = new Continent(tokens[0], stoi(tokens[1]));
            map->continents.push_back(c);
        }
        else if (territoryFlag && tokens.size() != 0) {
            Territory* t = new Territory(tokens[1], stoi(tokens[0]), 0);
            map->continents[stoi(tokens[2]) - 1]->territories.push_back(t);
            map->territories.push_back(t);
        }
        else if (edgeFlag && tokens.size() != 0) {
            Territory* start = map->findTerritory(stoi(tokens[0]));
            for (int i = 1; i < tokens.size(); i++) {
                Territory* stop = map->findTerritory(stoi(tokens[i]));
                Edge* e = new Edge(start, stop);
                map->edges.push_back(e);
            }
        }
        tokens.clear();
    }
    for (Edge* e : map->edges) {
        e->start->adjacentTerritories.push_back(e->stop);
    }
}



