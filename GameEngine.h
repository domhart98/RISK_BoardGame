#include <iostream>
#include <vector>
#include <algorithm>


using namespace std;

class Player;
class MapLoader;
class Card;
class CommandProcessor;
class Command;

class GameEngine {

public:
    GameEngine();

    enum class GameState {start, map_loaded, map_validated, players_added, assign_reinforcements, play, win, end};

    bool validateCommand(Command command);
    void start();
    void displayMainMenu();
    void assignReinforcements();
    void deckShuffle();
    void deckAdd(Card* cardToAdd);
    void deckRemove(int pos);
    
    GameState getState();
    void setState(GameEngine::GameState state);
    Command getCommand();
    void setCommand(string command);
    Player* getPlayer();
    vector<Card*> getDeck();
    MapLoader* getMapLoader();
    
    friend ostream &operator<<(ostream& out, const GameState gs);

    

private:
    
    GameState state;
    vector<Player*> players;
    vector<Card*> deck;
    MapLoader* mapLoader;
    CommandProcessor* cp;
    int numPlayers;
    int totalUnusedArmies;

    bool win;
};

