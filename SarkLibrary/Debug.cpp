#include "Debug.h"

namespace sark{

	//=============================================
	//		Debug::LogLevel implementation
	//=============================================

	Debug::LogLevel::LogLevel(int _lv, const char* _str) :lv(_lv), str(_str){}

	Debug::LogLevel::LogLevel(const LogLevel& logLv) : lv(logLv.lv), str(logLv.str){}

	bool Debug::LogLevel::operator==(const LogLevel& logLv) const{
		return (lv == logLv.lv);
	}
	bool Debug::LogLevel::operator!=(const LogLevel& logLv) const{
		return (lv != logLv.lv);
	}
	const std::string Debug::LogLevel::ToString() const{
		return str;
	}

	const Debug::LogLevel& Debug::LogLevel::Info(){
		static LogLevel _info(0, "Info");
		return _info;
	}
	const Debug::LogLevel& Debug::LogLevel::Warn(){
		static LogLevel _warn(1, "Warn");
		return _warn;
	}
	const Debug::LogLevel& Debug::LogLevel::Error(){
		static LogLevel _error(2, "Error");
		return _error;
	}
	const Debug::LogLevel& Debug::LogLevel::Fatal(){
		static LogLevel _fatal(3, "Fatal");
		return _fatal;
	}


	//=============================================
	//		Debug::LogElement implementation
	//=============================================

	Debug::LogElement::LogElement(const std::string& msg, const LogLevel& logLv,
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

	const std::string&		Debug::LogElement::GetMsg() const{ return mMsg; }
	const Debug::LogLevel&	Debug::LogElement::GetLogLevel() const{ return mLogLv; }
	const std::string&		Debug::LogElement::GetFileName() const{ return mFileName; }
	const std::string&		Debug::LogElement::GetFunctionName() const{ return mFuncName; }
	const int&				Debug::LogElement::GetLineNo() const{ return mLineNo; }
	const std::string&		Debug::LogElement::GetLogString() const{ return mLogString; }



	//=============================================
	//			Debug implementation
	//=============================================

	Debug::LogContainer Debug::mLogs;
	Debug::HaltHandler Debug::mHaltFunc = NULL;

	Debug::Debug(){}
	Debug::Debug(const Debug&){}


	void Debug::Log(const std::string& msg, const LogLevel& logLv,
		const char* fileName, int lineNo, const char* funcName){
		mLogs.push_back(LogElement(msg, logLv, fileName, lineNo, funcName));

		if (logLv == LogLevel::Fatal()){
			if (mHaltFunc != NULL){
				mHaltFunc(mLogs.back());
				exit(-1);
			}
		}
	}

	Debug::LogIterator Debug::GetLogBegin(){
		return mLogs.begin();
	}
	Debug::LogIterator Debug::GetLogEnd(){
		return mLogs.end();
	}

	void Debug::SetOnHaltHandler(HaltHandler onHalt){
		mHaltFunc = onHalt;
	}

}