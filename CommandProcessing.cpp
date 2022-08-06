#include "CommandProcessing.h"


//------------------------------------Command----------------------------------------

Command::Command(string cmd) {
    this->command = cmd;
}

Command::Command(const Command& c) {
    this->command = c.command;
    this->effect = c.effect;
}

Command& Command::operator=(const Command& c) {
    this->command = c.command;
    this->effect = c.effect;
    return *this;
}

ostream& operator<<(ostream& out, const Command& c) {
    out << "Command: " << c.command << "\nEffect: " << c.effect << endl;
    return out;
}

Command::~Command() {}


string Command::getCommand() {
    return this->command;
}

string Command::getEffect() {
    return this->effect;
};

void Command::setEffect(string effect) {
    this->effect = effect;
}

//------------------------------------CommmandProcessor----------------------------------------

CommandProcessor::CommandProcessor() {
}

CommandProcessor& CommandProcessor::operator=(const CommandProcessor& cp) {

    for (Command* c : cp.commands) {
        this->commands.push_back(c);
    }
    return *this;
}

CommandProcessor::CommandProcessor(const CommandProcessor& cp) {
    
    for (Command* c : cp.commands) {
        this->commands.push_back(new Command(*c));
    }
}

CommandProcessor::~CommandProcessor() {
    for (Command* c : this->commands) {
        delete c; 
        c = nullptr;
    }
    this->commands.clear();
}

//Reads new command from user, and saves it in the Command Processor
string CommandProcessor::readCommand() {
    string cmd;
    cout << "Enter command: " << endl;
    getline(cin, cmd);

    if (cmd == "end") { //user can exit application with the "end" command
        exit(0);
    }
    else {
        Command* command = new Command(cmd);
        this->commands.push_back(command);
        return cmd;
    }

}

//returns vector containing a sequence of Command objects. 
vector<Command*> CommandProcessor::getCommands() {
    return this->commands;
}


//------------------------------------FileCommandAdapterClass----------------------------------------

//Use file line reader to read, and then save, a list of commands from a text file.
string FileCommandProcessorAdapter::readCommand() {
    string cmd;
    cmd = flr->readLineFromFile();
    return cmd;
}

FileCommandProcessorAdapter::~FileCommandProcessorAdapter() {
    delete this->flr;
    flr = nullptr;
}


//------------------------------------FileLineReader----------------------------------------

FileLineReader::FileLineReader(string filename) {
    
    try {
        inputFileStream.open(filename);
        cout << "Map file " << filename << " was successfully opened" << endl;
    }
    catch (const ifstream::failure& e) {
        cout << "Error opening file" << endl;
        exit(0);
    }
}


string FileLineReader::readLineFromFile() {
    string command;

    if (inputFileStream.eof()) {
        cout << "End of file.\n";
        inputFileStream.close();
        return "eof";
    }

    getline(inputFileStream, command);
    cout << "Command: " << command << endl;
    return command;
}