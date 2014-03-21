#ifndef __DEBUG_HPP__
#define __DEBUG_HPP__

#include <list>
#include <string>
#include <chrono>

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
			LogLevel(int _lv, const char* _str) :lv(_lv), str(_str){}

		public:
			LogLevel(const LogLevel& logLv) : lv(logLv.lv), str(logLv.str){}

			inline bool operator==(const LogLevel& logLv) const{
				return (lv == logLv.lv);
			}
			inline bool operator!=(const LogLevel& logLv) const{
				return (lv != logLv.lv);
			}
			inline const std::string ToString() const{
				return str;
			}

			static inline const LogLevel& Info(){
				static LogLevel _info(0, "Info");
				return _info;
			}
			static inline const LogLevel& Warn(){
				static LogLevel _warn(1, "Warn");
				return _warn;
			}
			static inline const LogLevel& Error(){
				static LogLevel _error(2, "Error");
				return _error;
			}
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
				const char* fileName, int lineNo, const char* funcName)
				: mMsg(msg), mLogLv(logLv), mFileName(fileName), mLineNo(lineNo), mFuncName(funcName)
			{
				mLogString = "[" + mLogLv.ToString() + "] " + mMsg;

				if (mFileName != ""){
					mLogString += " - " + mFileName;

					if (mLineNo != -1)
						mLogString += "(" + std::to_string(mLineNo) + ")";
					if (mFuncName != "")
						mLogString += ", " + mFuncName;
				}
			}

			inline const std::string& GetMsg() const{ return mMsg; }
			inline const LogLevel& GetLogLevel() const{ return mLogLv; }
			inline const std::string& GetFileName() const{ return mFileName; }
			inline const std::string& GetFunctionName() const{ return mFuncName; }
			inline const int& GetLineNo() const{ return mLineNo; }
			inline const std::string& GetLogString() const{ return mLogString; }
		};

		typedef std::list<LogElement> LogContainer;
		typedef LogContainer::const_iterator LogIterator;

		typedef void(*ErrorHaltHandler)(const LogElement lastLog);

	private:
		static LogContainer mLogs;
		static ErrorHaltHandler mOnError;

		Debug(){}
		Debug(const Debug&){}

	public:
		static inline void Log(const std::string& msg, const LogLevel& logLv,
			const char* fileName, int lineNo, const char* funcName){
			mLogs.push_back(LogElement(msg, logLv, fileName, lineNo, funcName));
		}

		static inline LogIterator GetLogBegin(){
			return mLogs.begin();
		}
		static inline LogIterator GetLogEnd(){
			return mLogs.end();
		}

		static inline void SetOnErrorHandler(ErrorHaltHandler onErr){
			mOnError = onErr;
		}
		static inline void OnError(){
			if (mOnError != NULL){
				if (mLogs.size() != 0)
					mOnError(mLogs.back());
				else
					mOnError(LogElement("", LogLevel::Error(), "", -1, ""));
			}
		}
	};

	Debug::LogContainer Debug::mLogs;
	Debug::ErrorHaltHandler Debug::mOnError = NULL;


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
		#define LogInfo(msg) do{ \
			sark::Debug::Log(msg, sark::Debug::LogLevel::Info(), DBG_FILE, DBG_LINE, DBG_FUNCNAME); \
		}while(0)
	#else
		#define LogInfo(msg) do{}while(0)
	#endif

		// LogWarn macro
	#ifdef SARKLIB_DBGMODE
		#define LogWarn(msg) do{ \
			sark::Debug::Log(msg, sark::Debug::LogLevel::Warn(), DBG_FILE, DBG_LINE, DBG_FUNCNAME); \
		}while(0)
	#else
		#define LogWarn(msg) do{}while(0)
	#endif

		// LogError macro
	#ifdef SARKLIB_DBGMODE
		#define LogError(msg) do{ \
			sark::Debug::Log(msg, sark::Debug::LogLevel::Error(), DBG_FILE, DBG_LINE, DBG_FUNCNAME); \
			sark::Debug::OnError(); \
		}while(0)
	#else
		#define LogError(msg) do{}while(0)
	#endif

}
#endif