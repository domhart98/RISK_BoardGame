#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

class Iloggable;
class Subject;
class GameEngine;

class Command {
private:
	string command;
	string effect;
	

public:
	Command(string cmd);
	Command(const Command& c);
	Command& operator=(const Command& c);
	friend ostream& operator<<(ostream& out, const Command& c);
	~Command();

	string getCommand();
	string getEffect();
	void setEffect(string effect);
};

class CommandProcessor {
private:
	vector<Command*> commands;
	
public:
	

	CommandProcessor();
	CommandProcessor(const CommandProcessor& cp);
	CommandProcessor& operator=(const CommandProcessor& cp);
	~CommandProcessor();
	virtual string readCommand();
	void saveCommand(string cmd);
	vector<Command*> getCommands();
	
};


class FileLineReader {
private:
	ifstream inputFileStream;

public:
	FileLineReader(string filename);
	~FileLineReader() = default;
	string readLineFromFile();
};

class FileCommandProcessorAdapter : public CommandProcessor {
private:
	FileLineReader* flr;

public:
	FileCommandProcessorAdapter(FileLineReader* flr) { this->flr = flr; };
	~FileCommandProcessorAdapter();
	string readCommand();


};
