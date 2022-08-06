#include "GameEngine.h"
#include "CommandProcessing.h"
#include "Player.h"
#include "Map.h"
#include "Cards.h"
using namespace std;


GameEngine::GameEngine() {
    this->totalUnusedArmies = 0;
    this->numPlayers = 0;
    this->state = GameState::start;
    this->cp = new CommandProcessor();
    this->deck;
    this->mapLoader = new MapLoader();
    this->win = false;

}

void GameEngine::displayMainMenu() {
    cout << "Current State: " << this->getState() << endl;
    cout << "[Commands]" << "load map, validate map, add player, assign reinforcements, play, restart, end" << endl;
  
}

//Starts new game. Reads commands from user (selected from menu), until end condition is fulfilled.
void GameEngine::start() {

    cout << "New Game of RISK started..." << endl;
    
    while (this->getState() != GameState::end) {
        displayMainMenu();
        cp->readCommand();
        cout << this->validateCommand(*cp->getCommands().back()) << endl;
        
    }
    cout << "nigga";
    
}

//Creates Deck of Card objects using the loaded Map Territories. Distributes cards/territories amongst players. Prompts users to place armies, turn by turn.
void GameEngine::assignReinforcements() {
    //For every territory, there is a Card object containing the territory name plus the Unit Type ("Cavalry", "infantry", "Artillery").
    cout << "Generating Deck...." << endl;
    for (int i = 0; i < this->mapLoader->map->territories.size(); i++) {
        string unitType;
        if (i % 3 == 0) {
            unitType = "Cavalry";
        }
        else if (i % 3 == 2) {
            unitType = "Infantry";
        }
        else if (i % 3 == 1) {
            unitType = "Artillery";
        }
        Card* card = new Card(unitType, this->mapLoader->map->territories[i]->name);
        this->deckAdd(card);
    }

    //Shuffle deck and share cards/territories amongst the players.
    cout << "Distributing Territories...." << endl;
    this->deckShuffle();

    for (int i = 0; i < this->deck.size(); i++) {
        int playerNum = i % this->numPlayers;

        this->players[playerNum]->hand.push_back(this->deck[i]);
        for (Territory* territory : this->mapLoader->map->territories) {
            if (territory->name == this->deck[i]->country) {
                this->players[playerNum]->owned.push_back(territory);
                territory->owner = this->players[playerNum];
            }
        }
    }

    cout << "How would you like to assign reinforcements?" << endl;
    cout << "1. Automatically place 1 army on each territory, then each player adds the remaining armies in turn." << endl;
    cout << "2. Each player adds their armies turn by turn, and they must eventually place 1 army on each of their territories." << endl;

    int choice;
    cin >> choice;

    if (choice == 1) {
        for (Player* player : this->players) {

            //40 - etc.
            player->numArmies = 40 - ((this->players.size() - 2) * 5); //Give each player a number of armies, depending on how many players are in the game

            this->totalUnusedArmies += player->numArmies;

            for (Territory* terr : player->owned) {
                terr->armyNumber++; //Add army to territory
                player->numArmies--; //Remove said army from player
                this->totalUnusedArmies--; //Remove said army from pool of unused armies
            }

        }

        //Turn by turn, display the players territories and select which ones to place your armies on
        int i = 0;
        while (this->totalUnusedArmies > 0) {
            cout << "Select one of your territories to place armies." << endl;
            this->players[i % this->players.size()]->toDefend();

            string toDefend;
            cin >> toDefend;

            cout << "Player armies: " << this->players[i % this->players.size()]->numArmies << endl;
            cout << "How many armies to place here?: ";

            int toPlace;
            cin >> toPlace;

            for (Territory* terr : this->players[i % this->players.size()]->owned) {
                if (toDefend == terr->name) {
                    terr->armyNumber += toPlace;
                    this->players[i % this->players.size()]->numArmies -= toPlace;
                    this->totalUnusedArmies -= toPlace;
                }
            }
            i++;
        }

    }

    
}

//Takes Command object as parameter. Checks that the current command is allowed given the State of the Game.
//**compare() returns <0 if all compared characters match, but the COMPARED string is shorter.
bool GameEngine::validateCommand(Command command) {
    
    switch(this->getState()) {
        case GameState::start:
            if (command.getCommand().compare("load map") == 0 ) {
                this->mapLoader->loadMap("europe.map");
                state = GameState::map_loaded;
                command.setEffect("Map loaded.");
                return true;
            } else {
                cout << "invalid command" << endl;
                return false;
            }
            break;

        case GameState::map_loaded:
            if (command.getCommand().compare("load map") == 0) {
                
                state = GameState::map_loaded;
                command.setEffect("Map successfully loaded.");
                return true;
            } else if (command.getCommand().compare("validate map") == 0) {
                if (this->mapLoader->map->validate()) {
                    state = GameState::map_validated;
                    command.setEffect("Map is valid.");
                    cout << "Map successfully validated" << endl;
                }
                else {
                    cout << "Map is not valid." << endl;
                }
                
                return true;
            } else {
                cout << "invalid command" << endl;
                return false;
            }
            break;

        case GameState::map_validated:
            if (command.getCommand().compare("add player") == 0) {
                string pName = "p";
                pName += to_string(this->players.size());
                Player* player = new Player(pName);
                this->players.push_back(player);
                numPlayers++;
                state = GameState::players_added;
                command.setEffect("Player added.");
                cout << "Player successfully added" << endl;
                return true;
            } else {
                cout << "invalid command" << endl;
                return false;
            }
            break;

        case GameState::players_added:
            if (command.getCommand().compare("add player") == 0) {
                string pName = "p";
                pName += to_string(this->players.size());
                Player* player = new Player(pName);
                this->players.push_back(player);
                numPlayers++;
                state = GameState::players_added;
                command.setEffect("Player added.");
                cout << "Player successfully added" << endl;
                return true;
            } else if (command.getCommand().compare("assign reinforcements") == 0) {
                state = GameState::assign_reinforcements;
                
                this->assignReinforcements();

                command.setEffect("Reinforcements assigned.");
                cout << "Reinforcements successfully assigned" << endl << endl;
               
                return true;
            } else {
                cout << "invalid command" << endl;
                return false;
            }
            break;

        case GameState::assign_reinforcements:
            if (command.getCommand().compare("play") == 0) {
                state = GameState::play;
                command.setEffect("Game Phase started.");
                cout << "Game Started..." << endl;

                int turnNum = 0;
                while (!this->win) {
                    for (Player* p : this->players) {
                        //Add new armies based on currently owned Territories
                        p->numArmies += p->owned.size() / 3;
                        this->totalUnusedArmies += p->owned.size() / 3;
                        bool continentBonus = false;
                        
                        //If the all of the Territories of a Continent are owned by the current player, add bonus armies.
                        int i = 0;
                        for (Continent* c : this->mapLoader->map->continents) {
                            for (Territory* t : c->territories) {
                                if (t->owner->name == p->name) {
                                    i++;
                                }
                            }
                            if (i == c->territories.size()) {
                                p->numArmies += c->bonusPts;
                            }
                        }

                        //Distribute new armies amongst player territories
                        while (p->numArmies > 0) {
                            cout << "Select one of your territories to place armies." << endl;
                            p->toDefend();

                            string toDefend;
                            cin >> toDefend;

                            cout << "Player armies: " << p->numArmies << endl;
                            cout << "How many armies to place here?: ";

                            int toPlace;
                            cin >> toPlace;

                            for (Territory* terr : p->owned) {
                                if (toDefend == terr->name) {
                                    terr->armyNumber += toPlace;
                                    p->numArmies -= toPlace;
                                    this->totalUnusedArmies -= toPlace;
                                }
                            }

                        }

                        //display player menu for turn
                       
                        vector<Territory*> possibleTargets =  p->toAttack(); //possibleTargets contains all territories adjacent to player owned teritories
                        vector<Territory*> possibleAttackers;                
                        cout << "Select territory to attack: ";
                        
                        string choice;
                        cin >> choice;

                       /* Territory* toAttack;

                        for (Territory* terr : possibleTargets) {
                            if (choice == terr->name) {
                                toAttack = terr;
                            }
                        }

                        for (Territory* terr : p->owned) {
                            if (mapLoader->map->isAdjacent(terr, toAttack)) {
                                possibleAttackers.push_back(terr);     //possibleAttackers contains each owned territory that is adjacent to the territory being attacked
                            }
                        }

                        cout << "Adjacent owned territories:" << endl;
                        for (Territory* terr : possibleAttackers) {
                            cout << terr->name << " " << terr->armyNumber << ", ";
                        }
                        cout << endl << "From which territory would you like to attack?: ";
                        cin >> choice;
                        cout << endl << "With how many armies?: ";
                        int numAttackers;
                        cin >> numAttackers;

                        for (Territory* terr : possibleAttackers) {
                            if (choice == terr->name) {
                                p->attack(terr, toAttack);
                            }
                        }*/
                    }
                }

                return true;
            }
            else {
                cout << "invalid command" << endl;
                return false;
            }
            break;

        case GameState::play:
            if (command.getCommand().compare("win") == 0) {
                state = GameState::win;
                return true;
            }
            return false;
            break;

        case GameState::win:
            if (command.getCommand().compare("restart") == 0) {
                state = GameState::start;
                command.setEffect("Game restarted.");
                return true;
            } else if (command.getCommand().compare("end") == 0) {
                cout << "Game Ended." << endl;
                exit(0);
            } else {
                cout << "invalid command" << endl;
                return false;
            }
            break;
    }
}

void GameEngine::deckShuffle() {
    for (int i = 0; i < this->deck.size(); i++) {
        int j = i + rand() % (this->deck.size() - i);
        swap(this->deck[i], this->deck[j]);
    }
    return;
}

void GameEngine::deckAdd(Card* card) {
    this->deck.push_back(card);
}

void GameEngine::deckRemove(int pos) {
    int i = 0;
    for (auto deckIterator = deck.begin(); deckIterator != deck.end(); deckIterator++) {
        if (i == pos) {
            deck.erase(deckIterator);
            return;
        }
        i++;
    }
}

GameEngine::GameState GameEngine::getState() {
    return this->state;
}

void GameEngine::setState(GameEngine::GameState state) {
    this->state = state;
}

ostream& operator<<(ostream& out, const GameEngine::GameState gs) {
    switch (gs) {
    case GameEngine::GameState::start:
        cout << "start" << endl;
        break;

    case GameEngine::GameState::map_loaded:
        cout << "map_loaded" << endl;
        break;

    case GameEngine::GameState::map_validated:
        cout << "map_validated" << endl;
        break;

    case GameEngine::GameState::players_added:
        cout << "players_added" << endl;
        break;

    case GameEngine::GameState::assign_reinforcements:
        cout << "assign_reinforcements" << endl;
        break;

    case GameEngine::GameState::play:
        cout << "play" << endl;
        break;

    case GameEngine::GameState::win:
        cout << "win" << endl;
        break;
    }
    return out;
}