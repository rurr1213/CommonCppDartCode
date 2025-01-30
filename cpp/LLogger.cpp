#include <stdio.h>

#include "LLogger.h"

#include <string>
#include <iostream>

//LLogger* g_pLLogger = new LLogger();

Logger* g_pLogger = new LLogger();

using namespace std;

// -------------------------------------

LogEvent::LogEvent() :
	type(EVENTTYPE::INFO),
	name(""),
	description("")
//	peventData{0}
{
};

LogEvent::~LogEvent()
{
};

LogEvent::LogEvent(EVENTTYPE _type, string _name, string _des) :
	type(_type),
	name(_name),
	description(_des) {
};

LogEvent::LogEvent(std::uint64_t utcStartTime, int _timeMSecs, EVENTTYPE _type, string _name, string _des, string _file, string _func, int _line, int _value) :
	timeMSecs(_timeMSecs),
	type(_type),
	name(_name),
	description(_des)
{
	timeUTCMSecs = utcStartTime + _timeMSecs;
	file = _file;
	func = _func,
	line = _line;
	value = _value;
};

std::string LogEvent::toString(bool fullString) {
	double secs = timeMSecs / 1000.0;					// log delta time
	char timeBuf[128];
//	sprintf(timeBuf,"%.3f", secs);
	snprintf(timeBuf, 128, "%.3f", secs);
	std::string log = timeBuf;

	if (type==EVENTTYPE::INIT)
		log = to_string((double)(timeUTCMSecs/1000.0));	// display full start time

	switch (type) {
	case EVENTTYPE::INIT:
		log += " INIT:";
		break;
	case EVENTTYPE::SYSERROR:
		log += " ERR:";
		break;
	case EVENTTYPE::WARNING:
		log += " !! WAR:";
		break;
	case EVENTTYPE::NOTE:
		log += " !! NOTE:";
		break;
	case EVENTTYPE::INFO:
		log += " INFO:";
		break;
	case EVENTTYPE::DBG:
		log += " DBG:";
		break;
	case EVENTTYPE::DBGDATA:
		log += " DBGDATA:";
		log += description;
		return log;
		break;
	case EVENTTYPE::CON:
		log += " CON: ";
		log += description;
		return log;
		break;
	default:
		log += " ???:";
		break;

	}
	log += " " + name;
	if (description.size()>0)
		log += ", " + description;
	log += ": " + to_string(value);
	log += ", " + func + "()";
	if (fullString)
		log += "  : " + file;
	log += ", Line#" + to_string(line);
	return log;
};

json LLogEvent::to_json(void) {
	json jevent;
	double secs = timeMSecs / 1000.0;					// log delta time
	jevent[0] = secs;
	jevent[1] = type;
	jevent[2] = name;
	jevent[3] = description;
	jevent[4] = file;
	jevent[5] = func;
	jevent[6] = line;
	jevent[7] = value;
	if (type == EVENTTYPE::INIT) {
		jevent[8] = (double)(timeUTCMSecs / 1000.0);
	}
	return jevent;
}

bool LLogEvent::from_json(json jevent)
{
	try {
		timeMSecs = (int)((double)jevent[0] * 1000.0);
		type = jevent[1];
		name = jevent[2];
		description = jevent[3];
		file = jevent[4];
		func = jevent[5];
		line = jevent[6];
		value = jevent[7];
		if (type == EVENTTYPE::INIT) {
			timeUTCMSecs = (int)((double)jevent[8]*1000.0);
		}
	}
	catch (...) {
		assert(false);
	}
	return true;
}

// ------------------------------------------------

std::mutex LLogger::io_mutex;

LLogger::LLogger()
{
}

LLogger::~LLogger() {
}

bool LLogger::add(LogEvent botEvent)
{
	if ( botEvent.type > logLevel)
		return false;
		
	std::lock_guard<std::mutex> lk(io_mutex);

	botEvent.timeMSecs = (int) getRelativeTimeInMilliSecs();
	cout << botEvent.toString() << "\n";
	LLogEvent* pllogEvent = (LLogEvent*)&botEvent;
	std::string line = pllogEvent->to_json().dump();
	logBuffer.add(*pllogEvent);
	logFile << line << "\n";
	return true;
}

bool LLogger::add(LogEvent::EVENTTYPE type, string name, string desc) {
	LogEvent event(type, name, desc);
	return add(event);
}

bool LLogger::add(LogEvent::EVENTTYPE type, string name, string desc, string file, string func, int line, int value) {
	LogEvent event(0, 0, type, name, desc, file, func, line, value);
	return add(event);
}

void LLogger::setStateString(string keyName, string value, unsigned int flags) {
//	stateStore.setStateString(keyName, value, flags);
}

void LLogger::setStateInt(string keyName, int value, unsigned int flags)
{
//	stateStore.setStateInt(keyName, value, flags);
}

void LLogger::init() {
	getRelativeTimeInMilliSecs();
	try {
		logFile.open (logFileName, std::ofstream::out);
	} catch(...) {
	}
	return;
}

void LLogger::preDeinit(void) {
}

void LLogger::deinit(void) {
	logFile.flush();
	logFile.close();
}

void LLogger::printEventList(string title) {
//	eventList.debugPrintEvents();
}

bool LLogger::getLogLines(int startIndex, int numLines, std::list<std::string>& _list) {
	return logBuffer.getLogLines(startIndex, numLines, _list);
}

void LLogger::printf(LPCWSTR pFormat, ...) {
	/*
	va_list pArg;

	va_start(pArg, pFormat);
	enum {BOTBASEBUFSIZE=1000};
	WCHAR buf[BOTBASEBUFSIZE];
	int len = _vsntprintf_s(buf, BOTBASEBUFSIZE, BOTBASEBUFSIZE-1, pFormat, pArg);
	va_end(pArg);
	Cwstring line(buf);
	LOG_CON(line.to_string());
	*/
}

void LLogger::exception(const std::exception& e)
{
/*	Cwt::Cwtools tools;
	wstring ewhat = tools.getExceptionString(e);
	Logger::printf(L"Exception %s", ewhat);
	printEventList("Exception");
	assert(false);
	*/
}

void LLogger::windowsErr(string msg)
{
/*	Cwt::Cwtools tools;
	wstring lastError = tools.getLastErrorStringW();
	Logger::printf(L"%s, last error: %s", Cwstring(msg).c_wstr(), lastError.c_str());
	flushLogs("Exception");
	assert(false);
	*/
}

void LLogger::windowsErrD(string mark, string desc, int val)
{
/*	Cwt::Cwtools tools;
	wstring lastError = tools.getLastErrorStringW();
	Logger::printf(L"%s, %s, %d, last error: %s", Cwstring(mark).c_wstr(), Cwstring(desc).c_wstr(), val, lastError.c_str());
	flushLogs("Exception");
	assert(false);
	*/
}


void LLogger::flushLogs(std::string name)
{
	logFile.flush();
//	printEventList(name);
}
// ----------------------------------------------------

double LLogger::getCurrentTimeInSecs(void)
{
	/*
	struct timespec	spec;
	clock_gettime(CLOCK_REALTIME, &spec);

	double secs = (spec.tv_sec);
	secs += (spec.tv_nsec / 1000000000.0);
	return secs;
	*/
	time_t seconds;
	time(&seconds);
	return (int)seconds;
}

double LLogger::getRelativeTimeInMilliSecs(void)
{
	static double utcStartTime = 0;
	if (utcStartTime==0) {
		utcStartTime = getCurrentTimeInSecs();
		return 0;
	}
	double relativeTime = getCurrentTimeInSecs() - utcStartTime;
	return relativeTime;
}
