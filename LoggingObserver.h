#pragma once
#include <string>
#include <list>

// Declarations for the classes ILoggable, Subject, Observer and LogObserver, which all implement the Observer pattern.

class ILoggable
{
public:
	// pure virtual function which accepts a string and writes it into a log file.
	virtual void stringToLog(std::string info) = 0;
	~ILoggable();
	ILoggable();
};

class Observer
{
public:
	void Update(ILoggable& loggable, std::string info);
	~Observer();
	Observer();
};

class LogObserver : public Observer
{
public:
	void Update(ILoggable& loggable, std::string info);
	~LogObserver();
	LogObserver();
};

class Subject
{
public:
	virtual void Attach(Observer* o);
	virtual void Detach(Observer* o);
	void Notify(ILoggable& loggable, std::string info);
	~Subject();
	Subject();
private:
	std::list <Observer*>* _observers;
};