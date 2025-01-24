
#include <string.h>
#include <iostream>
#include <fstream> 
#include <mutex>
#include <list>

#include "json.hpp"

using json = nlohmann::json;

#include "Logger.h"

typedef wchar_t* LPWSTR, *PWSTR;
typedef const wchar_t* LPCWSTR;
/*
class LLogEvent : public LogEvent {
public:
	std::uint64_t timeUTCMSecs = {0};	// absolute UTC time in Mseconds, to merge data in future
	int timeMSecs = 0;	// time delay since start
	enum class EVENTTYPE {
		INIT = 0,
		SYSERROR,
		EXCEPTION,
		WARNING,
		NOTE,
		INFO,
		CON,			// just for diagnostics, log line to console
		DBG,				// Debug Messages
		DBGDATA,
	} type;
	enum EVENT {
		STARTED,
		ENDED,
	};
	std::string name;
	std::string description;
	std::string file;
	std::string func;
//	LogEventData* peventData = 0;
	int line = 0;
	int value = 0;
	LLogEvent();
	LLogEvent(EVENTTYPE _type, std::string _name, std::string _des);
	LLogEvent(std::uint64_t  utcStartTime, int _timeMSecs, EVENTTYPE _type, std::string _name, std::string _des, std::string _file, std::string _func, int _line, int _value);
	~LLogEvent();
	std::string toString(bool fullString = false);
	json toJson(void);
//	std::string toJsonString(void);
	bool fromJson(json jevent);
	operator std::string() { return toString(); }
};
*/

class LLogEvent : public LogEvent {
public:
	json to_json(void);
	bool from_json(json jevent);
};

class LogBuffer {
	const int MAXBUFF_SIZE = 1000;
    std::mutex mutex;
	std::list<LogEvent> eventList;
public:
	bool add(LogEvent logEvent) {
		std::lock_guard<std::mutex> lock(mutex);
		if ((int)eventList.size()>MAXBUFF_SIZE) {
			eventList.pop_front();
		}
		eventList.push_back(logEvent);
		return true;
	}
	bool getLogLines(int startIndex, int numLines, std::list<std::string>& _list) {
		bool moreAvailable = false;
		if (startIndex>=(int)eventList.size()) return moreAvailable;
		std::lock_guard<std::mutex> lock(mutex);
		int index = 0;
		for(auto item: eventList) {
			if (index>=startIndex) {
				_list.push_back(item);
			}
			index++;
			if ((int)_list.size()>=numLines) {
				moreAvailable = true;
				break;
			}
		}
		return moreAvailable;
	}
};


class LLogger : public Logger {
	static std::mutex io_mutex;
		double getCurrentTimeInSecs(void);
		double getRelativeTimeInMilliSecs();
		std::ofstream logFile;
		LogBuffer logBuffer;
    public:
		std::string logFileName = "logFile.json";
        static void log(std::string tag, std::string mark, std::string description, int value);
		virtual bool add(LogEvent botEvent);
		virtual bool add(LogEvent::EVENTTYPE type, std::string name, std::string desc);
		virtual bool add(LogEvent::EVENTTYPE type, std::string name, std::string desc, std::string file, std::string func, int line, int value);
		void setStateString(std::string keyName, std::string value, unsigned int flags = 1);
		void setStateInt(std::string keyName, int value, unsigned int flags = 1);
		LLogger();
		~LLogger();
		void init();
		void preDeinit(void);
		void deinit();
	//	void getStateMap(ThreadSafeStateMap::SafeStateMap& _stateMap) { stateStore.getThreadSafeStateMap(_stateMap);  }
	//	void setStateMapFlags(LOGFLAGS flags) { stateStore.setFlags(flags); }
	//	LOGFLAGS getStateMapFlags(void) { return stateStore.getFlags(); }
		virtual void process(void) {};
		bool processEvents(bool logToScreen = true);
		void printEventList(std::string title);
		void printf(LPCWSTR pFormat, ...);
		void exception(const std::exception& e);
		void windowsErr(std::string msg);
		void windowsErrD(std::string name, std::string desc, int val);
	//	void exportLogs(void);
	//	void uploadState(void);
	//	void uploadLogs(void);
	//	bool onEvent(Event e);
	//	bool checkOperations(void);
	//	bool checkOperationsPreDeinit(void);
		void flushLogs(std::string name = "flush");
		bool getLogLines(int startIndex, int numLines, std::list<std::string>& _list);
};

extern LLogger* g_pLLogger;

