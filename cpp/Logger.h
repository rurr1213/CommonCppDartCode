#pragma once
#include <stdlib.h>
#include <string>
#include <list>

class LogEventData {
public:
	enum class EVENTDATATYPE {
		INIT,			// initialization information - date, time, etc
	} dataType;
	int dataLength;
	void* pdata;
	LogEventData() :
		dataType{ EVENTDATATYPE::INIT },
		dataLength{0},
		pdata{0}
	{
	}
	~LogEventData() {
		if (pdata != 0) {
			delete (char *)pdata;
			pdata = 0;
		}
	}
};

class LogEvent {
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
	LogEventData* peventData = 0;
	int line = 0;
	int value = 0;
	LogEvent();
	LogEvent(EVENTTYPE _type, std::string _name, std::string _des);
	LogEvent(std::uint64_t  utcStartTime, int _timeMSecs, EVENTTYPE _type, std::string _name, std::string _des, std::string _file, std::string _func, int _line, int _value);
	~LogEvent();
	std::string toString(bool fullString = false);
//	json toJson(void);
//	bool fromJson(json jevent);
	operator std::string() { return toString(); }
};

class Logger {
public:
	Logger() {};
	~Logger() {};
	virtual void init() = 0;
	virtual void deinit(void) = 0;
	virtual void preDeinit(void) = 0;
	virtual bool add(LogEvent botEvent) = 0;
	virtual bool add(LogEvent::EVENTTYPE type, std::string name, std::string desc) = 0;
	virtual bool add(LogEvent::EVENTTYPE type, std::string name, std::string desc, std::string file, std::string func, int line, int value) = 0;
	virtual void setStateString(std::string keyName, std::string value, unsigned int flags = 1) = 0;
	virtual void setStateInt(std::string keyName, int value, unsigned int flags = 1) = 0;
	virtual void printEventList(std::string title) = 0;
	virtual void printf(wchar_t const* pFormat, ...) = 0;
	virtual void exception(const std::exception& e) = 0;
	virtual void windowsErr(std::string msg) = 0;
	virtual void windowsErrD(std::string name, std::string desc, int val) = 0;
	virtual void process(void) = 0;
	virtual void flushLogs(std::string name = "flush") = 0;
	void removeAnyJsonDelimiters(std::string& _string) {
		char* p = (char*)_string.c_str();
		size_t len = _string.length();
		while (len-- > 0) {
			char c = *p;
			switch (c) {
			case '{':
				*p = '[';
				break;
			case '}':
				*p = ']';
				break;
			case ':':
				*p = '=';
				break;
			case '\"':
				*p = ' ';
				break;
			default:
				break;
			}
			p++;
		}
	}
	virtual bool getLogLines(int startIndex, int numLines, std::list<std::string>& _list) { return false;};
};

extern Logger* g_pLogger;

#define LOG_GP()	g_pLogger

#define LOG_NOTIFY(type, name, desc)	LOG_GP()->add(type, name, desc, __FILE__, __func__, __LINE__)

#define LOG_ASSERT(call) if(!(call)) LOG_GP()->add(LogEvent::EVENTTYPE::SYSERROR, #call, "failed", __FILE__, __func__, __LINE__, call)
#define LOG_ASSERTCOM(call, comm) if(!call) LOG_GP()->add(LogEvent::EVENTTYPE::SYSERROR, #call, comm, __FILE__, __func__, __LINE__, call)
#define LOG_ASSERTCOMVAL(call, comm, val) if(!call) LOG_GP()->add(LogEvent::EVENTTYPE::SYSERROR, #call, comm, __FILE__, __func__, __LINE__, val)
//#define LOG_INFO(mark, val) LOG_GP()->add(LogEvent::EVENTTYPE::INFO, mark, "", __FILE__, __func__, __LINE__, val)
#define LOG_INFO(mark, desc, val) LOG_GP()->add(LogEvent::EVENTTYPE::INFO, mark, desc, __FILE__, __func__, __LINE__, val)
#define LOG_NOTE(mark, desc, val) LOG_GP()->add(LogEvent::EVENTTYPE::NOTE, mark, desc, __FILE__, __func__, __LINE__, val)
#define LOG_WARNING(mark, desc, val) LOG_GP()->add(LogEvent::EVENTTYPE::WARNING, mark, desc, __FILE__, __func__, __LINE__, val)
#define LOG_ERROR(mark, desc, val) LOG_GP()->add(LogEvent::EVENTTYPE::SYSERROR, mark, desc, __FILE__, __func__, __LINE__, val)
#define LOG_CON(desc) LOG_GP()->add(LogEvent::EVENTTYPE::CON, "", desc, __FILE__, __func__, __LINE__, 0)
#define LOG_DBG(mark, desc, val) LOG_GP()->add(LogEvent::EVENTTYPE::DBG, mark, desc, __FILE__, __func__, __LINE__, val)
#define LOG_DBGD(mark, desc, val) LOG_GP()->add(LogEvent::EVENTTYPE::DBGDATA, mark, desc, __FILE__, __func__, __LINE__, val)

#define LOG_STATESTRING(key, val) LOG_GP()->setStateString(key, val)
#define LOG_STATEINT(key, val) LOG_GP()->setStateInt(key, val)
#define LOG_STATEINTF(key, val, flags) LOG_GP()->setStateInt(key, val, flags)

#define LOG_PRINTEVENTLIST(val) LOG_GP()->printEventList(val)

#define LOG_PRINTF(...)	LOG_GP()->printf(__VA_ARGS__)
#define LOG_EXCEPTION(...) LOG_GP()->printf(__VA_ARGS__)
#define LOG_EXCEPTION_CODE(e) LOG_GP()->exception(e)
#define LOG_WINDOWSERROR(msg) LOG_GP()->windowsErr(msg)
#define LOG_WINDOWSERRORD(mark, desc, val) LOG_GP()->windowsErrD(mark, desc, val)

#define LOG_FLUSH(_word) LOG_GP()->flushLogs(_word)
#define LOG_PREDEINIT() LOG_GP()->preDeinit()
#define LOG_PROCESS() LOG_GP()->process()

#define LOG_REMOVEJSON(_word) LOG_GP()->removeAnyJsonDelimiters(_word)
#define LOG_GETLOGLINES(start, num, list) LOG_GP()->getLogLines(start, num, list)

