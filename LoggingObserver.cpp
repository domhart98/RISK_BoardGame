#include <iostream>
#include <fstream>
#include "LoggingObserver.h"

using namespace std;

// The following are constructor/destructor stubs. Empty because
// the classes lack variables.

ILoggable::ILoggable() {
}


ILoggable::~ILoggable() {
}

Observer::~Observer() {
}

Observer::Observer() {
}

LogObserver::~LogObserver() {}
LogObserver::LogObserver() {}

// Calls string-to-log function to update the log with new information.

void Observer::Update(ILoggable& loggable, string info)
{
	loggable.stringToLog(info);
}


Subject::Subject()
{
	_observers = new list<Observer*>;
}

Subject::~Subject()
{
	delete _observers;
}


void Subject::Attach(Observer* o)
{
	_observers->push_back(o);
}
void Subject::Detach(Observer* o)
{
	_observers->remove(o);
}

// Calls the Update function.

void Subject::Notify(ILoggable& loggable, string info)
{
	list<Observer*>::iterator i = _observers->begin();
	for (; i != _observers->end(); ++i)
		(*i)->Update(loggable, info);

}