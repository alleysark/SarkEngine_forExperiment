#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <list>
#include <string>

// *note: do not include any sark library headers

namespace sark{

	// it defines library compiling mode as 'debug'.
	// if it does not defined, some functions in Debug
	// are just skipped on compile time
	#define SARKLIB_DBGMODE


	// debug helper class
	// it makes users to logging and debugging system info
	class Debug{
	public:
		class LogLevel{
		private:
			int lv;
			const char* str;
			LogLevel(int _lv, const char* _str);

		public:
			LogLevel(const LogLevel& logLv);

			bool operator==(const LogLevel& logLv) const;
			bool operator!=(const LogLevel& logLv) const;
			const std::string ToString() const;

			static const LogLevel& Info();
			static const LogLevel& Warn();
			static const LogLevel& Error();
			static const LogLevel& Fatal();
		};

		class LogElement{
		private:
			std::string mMsg;
			LogLevel mLogLv;

			std::string mFileName;
			int mLineNo;
			std::string mFuncName;

			std::string mLogString;

		public:
			LogElement(const std::string& msg, const LogLevel& logLv,
				const char* fileName, int lineNo, const char* funcName);

			const std::string& GetMsg() const;
			const LogLevel& GetLogLevel() const;
			const std::string& GetFileName() const;
			const std::string& GetFunctionName() const;
			const int& GetLineNo() const;
			const std::string& GetLogString() const;
		};

		typedef std::list<LogElement> LogContainer;
		typedef LogContainer::const_iterator LogIterator;

		typedef void(*HaltHandler)(const LogElement lastLog);

	private:
		static LogContainer mLogs;
		static HaltHandler mHaltFunc;

		Debug();
		Debug(const Debug&);

	public:
		// log message with log-info
		// if user log for FATAL issue, it'll call the exit() after onHalt().
		static void Log(const std::string& msg, const LogLevel& logLv,
			const char* fileName, int lineNo, const char* funcName);

		static LogIterator GetLogBegin();
		static LogIterator GetLogEnd();

		static void SetOnHaltHandler(HaltHandler onHalt);
	};
	

	// debug infos redefinations

	#define DBG_FILE __FILE__
	#define DBG_LINE __LINE__

	#ifdef _MSC_VER
		#define DBG_FUNCNAME __FUNCTION__
	#else
		#define DBG_FUNCNAME ""
	#endif


	// LogInfo macro
	#ifdef SARKLIB_DBGMODE
		#define LogInfo(msg) sark::Debug::Log(msg, sark::Debug::LogLevel::Info(), DBG_FILE, DBG_LINE, DBG_FUNCNAME)
	#else
		#define LogInfo(msg)
	#endif

	// LogWarn macro
	#ifdef SARKLIB_DBGMODE
		#define LogWarn(msg) sark::Debug::Log(msg, sark::Debug::LogLevel::Warn(), DBG_FILE, DBG_LINE, DBG_FUNCNAME)
	#else
		#define LogWarn(msg)
	#endif

	// LogError macro
	#ifdef SARKLIB_DBGMODE
		#define LogError(msg) sark::Debug::Log(msg, sark::Debug::LogLevel::Error(), DBG_FILE, DBG_LINE, DBG_FUNCNAME)
	#else
		#define LogError(msg)
	#endif

	// LogError macro
	#ifdef SARKLIB_DBGMODE
		#define LogFatal(msg) sark::Debug::Log(msg, sark::Debug::LogLevel::Fatal(), DBG_FILE, DBG_LINE, DBG_FUNCNAME)
	#else
		#define LogFatal(msg)
	#endif

}
#endif