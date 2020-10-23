#ifndef Exception_h
#define Exception_h

#include "stdio.h"

#include "Port.h"
#include <string>

namespace Common
{ 

class Exception
{
private:
	unsigned int mUi32ExceptionCode;
public:
	Exception(unsigned int aUi32Code = MultiChar("excp"))
		:mUi32ExceptionCode(aUi32Code)
	{
	}
	virtual ~Exception() {}

	inline const unsigned int& GetCode() { return mUi32ExceptionCode; }

	virtual const char* Message() const { return ""; }
};

class MessageException : public Exception
{
public:
	enum { MaxLen = 255 };
private:
	char mCMessage[MaxLen];
	size_t mILen;
public:
	MessageException(const char *aC, unsigned int aUi32Code = MultiChar("excp"))
		:Exception(aUi32Code)
	{
		strncpy(mCMessage, aC, MaxLen);
		mILen = strlen(mCMessage);
	}

	virtual const char* Message() const { return mCMessage; }

	inline MessageException& operator<<(const char *aC)
	{
		strncat(mCMessage, aC, MaxLen-mILen);
		mILen = strlen(mCMessage);
		return *this;
	}

	inline MessageException& operator<<(int aI)
	{
		char lC[50];
		sprintf(lC, "%i", aI);
		return operator<<(lC);
	}

	inline MessageException& operator<<(const double &aD)
	{
		char lC[50];
		sprintf(lC, "%g", aD);
		return operator<<(aD);
	}

	inline MessageException& operator<<(const float &aF)
	{
		char lC[50];
		sprintf(lC, "%g", aF);
		return operator<<(aF);
	}

	inline MessageException& operator<<(const std::string &aS)
	{
		return operator<<(aS.c_str());
	}


};

}

#endif
