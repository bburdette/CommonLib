#ifndef NVThing_h
#define NVThing_h

#include "Archivable.h"
#include "Exception.h"
#include <string>
#include <map>
#include <iostream>
#include "StringUtils.h"

using namespace std;

#define DONT_INCLUDE_WINDOWS_H
#include "Port.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// 'Name-Value' thing.
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Heirarchical Name-Value pairs, ie you can assocate strings with names within an
// NVThing, and you can take an NVThing and put it inside another NVThing.
// Functions are provided to storing other types.
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Important Regarding streams!
// In microsoft vc++ at least you must read in streams in binary mode.  Maybe a bug, who
// knows, but that's what you gotta do.
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

class NVArchivable;

class NVThing : public Archivable
{
public:
	NVThing(int aILevel = 0);
	NVThing(const NVThing &aNvt);
	virtual ~NVThing();

	NVThing& operator=(const NVThing &aNvt);

	void Clear();
	bool IsEmpty() const { return mMValues.empty(); }

	// - When you Add an NVThing, the parent takes over ownership
	// of that object.  Do not then add the NVThing to another
	// NVThing, this will result in a double delete later on.
	// - Also you are not allowed to read from a stream into an
	// NVThing you just added, only a 'base level' NVThing.
	inline void Add(const char *aCName, NVThing *aNvt);
	inline bool Find(const char *aCName, NVThing *&aNvt, bool aBNotFoundException = false) const;

	void Add(const char *aCName, const NVArchivable &aNva);
	inline bool Find(const char *aCName, NVArchivable &aNva, bool aBNotFoundException = false) const;

	// Add for C string.
	inline void Add(const char *aCName, const char* aCValue);
	inline bool Find(const char *aCName, const char *&aCValue, bool aBNotFoundException = false) const;
	// Add for various types.  These all end up calling Add for C string.
	inline void Add(const char *aCName, const string &aSValue);
	inline bool Find(const char *aCName, string &aSValue, bool aBNotFoundException = false) const;
	inline void Add(const char *aCName, const int64_t &aI64);
	inline bool Find(const char *aCName, int64_t &aI64, bool aBNotFoundException = false) const;
	inline void Add(const char *aCName, const int &aI);
	inline bool Find(const char *aCName, int &aI, bool aBNotFoundException = false) const;
	inline void Add(const char *aCName, const unsigned int &aUi);
	inline bool Find(const char *aCName, unsigned int &aUi, bool aBNotFoundException = false) const;
	inline void Add(const char *aCName, const short &aI);
	inline bool Find(const char *aCName, short &aI, bool aBNotFoundException = false) const;
	inline void Add(const char *aCName, const double &aD);
	inline bool Find(const char *aCName, double &aD, bool aBNotFoundException = false) const;
	inline void Add(const char *aCName, const float &aD);
	inline bool Find(const char *aCName, float &aD, bool aBNotFoundException = false) const;
	inline void Add(const char *aCName, const char aC);
	inline bool Find(const char *aCName, char &aC, bool aBNotFoundException = false) const;
	inline void Add(const char *aCName, bool aB);
	inline bool Find(const char *aCName, bool &aB, bool aBNotFoundException = false) const;

	// Deletes the first item with aCName.  Returns false if no item found.
	inline bool Delete(const char *aCName);
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Index Versions of the ftns.
	inline void Add(const char *aCName, int aIIndex, const char* aCValue);
	inline bool Find(const char *aCName, int aIIndex, const char *&aCValue, bool aBNotFoundException = false) const;
	inline void Add(const char *aCName, int aIIndex, const string &aSValue);
	inline bool Find(const char *aCName, int aIIndex, string &aSValue, bool aBNotFoundException = false) const;
	inline void Add(const char *aCName, int aIIndex, NVThing *aNvt);
	inline bool Find(const char *aCName, int aIIndex, NVThing *&aNvt, bool aBNotFoundException = false) const;
	inline void Add(const char *aCName, int aIIndex, const int64_t &aI64);
	inline bool Find(const char *aCName, int aIIndex, int64_t &aI64, bool aBNotFoundException = false) const;
	inline void Add(const char *aCName, int aIIndex, const int &aI);
	inline bool Find(const char *aCName, int aIIndex, int &aI, bool aBNotFoundException = false) const;
	inline void Add(const char *aCName, int aIIndex, const unsigned int &aI);
	inline bool Find(const char *aCName, int aIIndex, unsigned int &aI, bool aBNotFoundException = false) const;
	inline void Add(const char *aCName, int aIIndex, const short &aI);
	inline bool Find(const char *aCName, int aIIndex, short &aI, bool aBNotFoundException = false) const;
	inline void Add(const char *aCName, int aIIndex, const double &aD);
	inline bool Find(const char *aCName, int aIIndex, double &aD, bool aBNotFoundException = false) const;
	inline void Add(const char *aCName, int aIIndex, const char aC);
	inline bool Find(const char *aCName, int aIIndex, char &aC, bool aBNotFoundException = false) const;
	inline void Add(const char *aCName, int aIIndex, const bool aB);
	// No default here because of ambiguity with int Find.
	inline bool Find(const char *aCName, int aIIndex, bool &aB, bool aBNotFoundException) const;
	inline bool Delete(const char *aCName, int aIIndex);
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// You can call FindNext repeatedly with the same name to get subsequent items
	// with that name.  Call ResetFindNext to return to the first item.
	// Calls to regular Find() will not affect the FindNext iterator.
	// Use like this:
	// lNvt.ResetFindNext();
	// while (lNvt.FindNext("someitem", lIValue))
	// {
	//		cout << lIValue << endl;
	// }
	// Or do this:
	// lNvt.Find("someitem", lIValue);
	// cout << lIValue << endl;
	// while (lNvt.FindNext("someitem", lIValue))
	// {
	//		cout << lIValue << endl;
	// }
	//
	// You should make a point of calling the ResetFindNext() function, otherwise
	// you could end up starting in the middle of a series when you expect to be at the
	// beginning.  Also, FindNext() is not multithread friendly.  Only one FindNext at a
	// time per NVT.
	inline void ResetFindNext() const;

	inline bool FindNext(const char *aCName, NVThing *&aNvt, bool aBNotFoundException = false) const;

	bool FindNext(const char *aCName, NVArchivable &aNva, bool aBNotFoundException = false) const;

	inline bool FindNext(const char *aCName, const char *&aCValue, bool aBNotFoundException = false) const;
	inline bool FindNext(const char *aCName, string &aSValue, bool aBNotFoundException = false) const;
	inline bool FindNext(const char *aCName, int64_t &aI64, bool aBNotFoundException = false) const;
	inline bool FindNext(const char *aCName, int &aI, bool aBNotFoundException = false) const;
	inline bool FindNext(const char *aCName, unsigned int &aUi, bool aBNotFoundException = false) const;
	inline bool FindNext(const char *aCName, short &aI, bool aBNotFoundException = false) const;
	inline bool FindNext(const char *aCName, double &aD, bool aBNotFoundException = false) const;
	inline bool FindNext(const char *aCName, float &aD, bool aBNotFoundException = false) const;
	inline bool FindNext(const char *aCName, char &aC, bool aBNotFoundException = false) const;
	inline bool FindNext(const char *aCName, bool &aB, bool aBNotFoundException = false) const;
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


	// Old-style iostream stuff.
	void ToOs(ostream &aOs) const;
	void FromIs(istream &aIs);

	// New school, XML.
	/*
	void ToXml(ostream &aOs) const;
	void FromXml(istream &aIs);

	void FromXml(const TiXmlNode &aTxn);
	void ToXml(TiXmlNode &aTxn) const;
	*/
private:
	static const char *sCVersion;

	void ToOs_Internal(ostream &aOs) const;
	void FromIs_Internal(istream &aIs);

	void FromIs_Old(istream &aIs);

	class NVData
	{
	public:
		string* mS;
		NVThing* mNvt;
		NVData(const char* aC)
		{
			mS = new string(aC);
			replace_all(*mS, "&#x0A;", "\n");
			mNvt = 0;
		}
		NVData(NVThing *aNvt)
		{
			mS = 0;
			mNvt = aNvt;
		}

		~NVData()
		{
			if (mS)
				delete mS;
			if (mNvt)
				delete mNvt;
		}
	};
	typedef multimap<string, NVData*> NVMapType;
	NVMapType mMValues;
	mutable int mILevel;
	mutable string mSWk;

	// For repeated calls to Find.
	mutable NVMapType::const_iterator mIter;
	mutable string mSLastFind;

	inline const char* IndexName(const char *aCName, int aIIndex) const
	{
		mSWk = aCName;
		char lC[30];
        sprintf(lC, "%d", aIIndex);
		mSWk += lC;
		return mSWk.c_str();
	}

	inline static void StringOut(ostream &aOs, const string &aS)
	{
		const char *lC = aS.c_str();
		while (*lC != '\0')
		{
			if (*lC == '\n')
				aOs << "&#x0A;";
			else if (*lC == '\r')
				;
			else
				aOs << *lC;
			++lC;
		}
	}
};

// ---------------------------------------------------------
// NVArchivable ancestor.  Make any object Add/Find able with
// this ancestor.
// ---------------------------------------------------------

class NVArchivable
{
public:
	virtual ~NVArchivable() {}
	virtual void ToNvt(NVThing &aNvt) const = 0;
	virtual void FromNvt(const NVThing &aNvt) = 0;
	//virtual void ToNvt(NVThing &aNvt) const;
	//virtual void FromNvt(const NVThing &aNvt);
};

/*
void NewObj::ToNvt(NVThing &aNvt) const
{
}
void NewObj::FromNvt(const NVThing &aNvt)
{
}
*/

// ---------------------------------------------------------
// Implementations of the inline ftns.
// ---------------------------------------------------------

inline void NVThing::Add(const char *aCName, const char* aCValue)
{
	mMValues.insert(NVMapType::value_type(aCName, new NVData(aCValue)));
}

inline bool NVThing::Find(const char *aCName, const char* &aCValue, bool aBNotFoundException) const
{
	mIter = mMValues.find(aCName);
	// Do this?  Eliminates need for ResetFindNext().
	mSLastFind = aCName;

	if (mIter != mMValues.end() && mIter->second->mS)
	{
		aCValue = mIter->second->mS->c_str();
		return true;
	}
	else if (aBNotFoundException)
	{
		throw Common::MessageException(aCName) << " was not found";
	}
	else
		return false;		// not found or not a string.
}

inline void NVThing::Add(const char *aCName, const NVArchivable &aNva)
{
	NVThing *lNvt = new NVThing;
	aNva.ToNvt(*lNvt);
	Add(aCName, lNvt);
}

inline bool NVThing::Find(const char *aCName, NVArchivable &aNva, bool aBNotFoundException) const
{
	NVThing *lNvt;
	if (Find(aCName, lNvt))
	{
		aNva.FromNvt(*lNvt);
		return true;
	}
	else
		return false;
}

void NVThing::Add(const char *aCName, const string &aSValue)
{
	Add(aCName, aSValue.c_str());
}
bool NVThing::Find(const char *aCName, string &aSValue, bool aBNotFoundException) const
{
	const char *lC;
	if (Find(aCName, lC, aBNotFoundException))
	{
		aSValue = lC;
		return true;
	}
	else
		return false;
}

inline void NVThing::Add(const char *aCName, NVThing *aNvt)
{
	//aNvt->mILevel = mILevel + 1;		// do this on output.
	mMValues.insert(NVMapType::value_type(aCName, new NVData(aNvt)));
}

inline bool NVThing::Find(const char *aCName, NVThing *&aNvt, bool aBNotFoundException) const
{
	mIter = mMValues.find(aCName);
	// Do this?  Eliminates need for ResetFindNext().
	mSLastFind = aCName;

	if (mIter != mMValues.end() && mIter->second->mNvt)
	{
		aNvt = mIter->second->mNvt;
		return true;
	}
	else if (aBNotFoundException)
	{
		throw Common::MessageException(aCName) << " was not found";
	}
	else
		return false;		// not found or not a string.
}

inline void NVThing::Add(const char *aCName, const int &aI)
{
	char lC[100];
    sprintf(lC, "%d", aI);
	Add(aCName, lC);
}
inline bool NVThing::Find(const char *aCName, int &aI, bool aBNotFoundException) const
{
	const char *lC;
	if (Find(aCName, lC, aBNotFoundException))
	{
		aI = atoi(lC);
		return true;
	}
	else
		return false;
}

inline void NVThing::Add(const char *aCName, const short &aS)
{
	Add(aCName, (int)aS);
}
inline bool NVThing::Find(const char *aCName, short &aS, bool aBNotFoundException) const
{
	int lI;
	if (Find(aCName, lI, aBNotFoundException))
	{
		aS = (short)lI;
		return true;
	}
	else
		return false;
}

inline void NVThing::Add(const char *aCName, const unsigned int &aUi)
{
	char lC[100];
    sprintf(lC, "%d", aUi);
	Add(aCName, lC);
}
inline bool NVThing::Find(const char *aCName, unsigned int &aUi, bool aBNotFoundException) const
{
	const char *lC;
	if (Find(aCName, lC, aBNotFoundException))
	{
		aUi = atoi(lC);
		return true;
	}
	else
		return false;
}

inline void NVThing::Add(const char *aCName, const char aC)
{
	char lC[2];
	lC[0] = aC;
	lC[1] = '\0';
	Add(aCName, lC);
}

inline bool NVThing::Find(const char *aCName, char &aC, bool aBNotFoundException) const
{
	const char *lC;
	if (Find(aCName, lC, aBNotFoundException))
	{
		aC = *lC;
		return true;
	}
	else
		return false;
}

void NVThing::Add(const char *aCName, const double &aD)
{
	char lC[30];
	sprintf(lC, "%.16g", aD);
    Add(aCName, lC);
}
bool NVThing::Find(const char *aCName, double &aD, bool aBNotFoundException) const
{
	const char *lC;
	if (Find(aCName, lC, aBNotFoundException))
	{
		aD = atof(lC);
		return true;
	}
	else
		return false;
}

void NVThing::Add(const char *aCName, const float &aF)
{
	char lC[30];
	sprintf(lC, "%.16g", aF);
    Add(aCName, lC);
}
bool NVThing::Find(const char *aCName, float &aF, bool aBNotFoundException) const
{
	const char *lC;
	if (Find(aCName, lC, aBNotFoundException))
	{
		aF = static_cast<float>(atof(lC));
		return true;
	}
	else
		return false;
}

inline void NVThing::Add(const char *aCName, const int64_t &aI64)
{
	char lC[100];
//    sprintf(lC, "%" PRIi64, aI64);   // well i tried.  not compatable w cpp compilers it seems.
    sprintf(lC, "%li", aI64);
	Add(aCName, lC);
}

inline bool NVThing::Find(const char *aCName, int64_t &aI64, bool aBNotFoundException) const
{
	const char *lC;
	if (Find(aCName, lC, aBNotFoundException))
	{
		aI64 = atoll(lC);
		return true;
	}
	else
		return false;
}

inline void NVThing::Add(const char *aCName, bool aB)
{
	if (aB)
		Add(aCName, "true");
	else
		Add(aCName, "false");
}

inline bool NVThing::Find(const char *aCName, bool &aB, bool aBNotFoundException) const
{
	const char *lC;
	if (Find(aCName, lC, aBNotFoundException))
	{
		if (strncmp(lC, "true", 4) == 0)
			aB = true;
		else
			aB = false;

		return true;
	}
	else
		return false;
}

bool NVThing::Delete(const char *aCName)
{
	NVMapType::iterator lIter = mMValues.find(aCName);
	if (lIter != mMValues.end())
	{
		delete lIter->second;
		mMValues.erase(lIter);
		mIter = mMValues.end();
		return true;
	}
	else
		return false;		// not found.
}

// ------------------------------------------------------------------------------------------
// "Index" functions.
// ------------------------------------------------------------------------------------------
inline void NVThing::Add(const char *aCName, int aIIndex, const char* aCValue)
{
	Add(IndexName(aCName, aIIndex), aCValue);
}
inline bool NVThing::Find(const char *aCName, int aIIndex, const char *&aCValue, bool aBNotFoundException) const
{
	return Find(IndexName(aCName, aIIndex), aCValue, aBNotFoundException);
}
void NVThing::Add(const char *aCName, int aIIndex, const string &aSValue)
{
	Add(aCName, aIIndex, aSValue.c_str());
}
bool NVThing::Find(const char *aCName, int aIIndex, string &aSValue, bool aBNotFoundException) const
{
	const char *lC;
	if (Find(aCName, aIIndex, lC, aBNotFoundException))
	{
		aSValue = lC;
		return true;
	}
	else
		return false;
}
inline void NVThing::Add(const char *aCName, int aIIndex, NVThing *aNvt)
{
	Add(IndexName(aCName, aIIndex), aNvt);
}
inline bool NVThing::Find(const char *aCName, int aIIndex, NVThing *&aNvt, bool aBNotFoundException) const
{
	return Find(IndexName(aCName, aIIndex), aNvt, aBNotFoundException);
}

inline void NVThing::Add(const char *aCName, int aIIndex, const int64_t &aI64)
{
	Add(IndexName(aCName, aIIndex), aI64);
}
inline bool NVThing::Find(const char *aCName, int aIIndex, int64_t &aI64, bool aBNotFoundException) const
{
	return Find(IndexName(aCName, aIIndex), aI64, aBNotFoundException);
}

inline void NVThing::Add(const char *aCName, int aIIndex, const int &aI)
{
	Add(IndexName(aCName, aIIndex), aI);
}
inline bool NVThing::Find(const char *aCName, int aIIndex, int &aI, bool aBNotFoundException) const
{
	return Find(IndexName(aCName, aIIndex), aI, aBNotFoundException);
}

inline void NVThing::Add(const char *aCName, int aIIndex, const unsigned int &aUi)
{
	Add(IndexName(aCName, aIIndex), aUi);
}
inline bool NVThing::Find(const char *aCName, int aIIndex, unsigned int &aUi, bool aBNotFoundException) const
{
	return Find(IndexName(aCName, aIIndex), aUi, aBNotFoundException);
}

inline void NVThing::Add(const char *aCName, int aIIndex, const short &aS)
{
	Add(IndexName(aCName, aIIndex), aS);
}
inline bool NVThing::Find(const char *aCName, int aIIndex, short &aS, bool aBNotFoundException) const
{
	return Find(IndexName(aCName, aIIndex), aS, aBNotFoundException);
}

inline void NVThing::Add(const char *aCName, int aIIndex, const char aC)
{
	Add(IndexName(aCName, aIIndex), aC);
}
inline bool NVThing::Find(const char *aCName, int aIIndex, char &aC, bool aBNotFoundException) const
{
	return Find(IndexName(aCName, aIIndex), aC, aBNotFoundException);
}

inline void NVThing::Add(const char *aCName, int aIIndex, const bool aB)
{
	Add(IndexName(aCName, aIIndex), aB);
}
inline bool NVThing::Find(const char *aCName, int aIIndex, bool &aB, bool aBNotFoundException) const
{
	return Find(IndexName(aCName, aIIndex), aB, aBNotFoundException);
}

inline void NVThing::Add(const char *aCName, int aIIndex, const double &aD)
{
	Add(IndexName(aCName, aIIndex), aD);
}
inline bool NVThing::Find(const char *aCName, int aIIndex, double &aD, bool aBNotFoundException) const
{
	return Find(IndexName(aCName, aIIndex), aD, aBNotFoundException);
}

inline bool NVThing::Delete(const char *aCName, int aIIndex)
{
	return Delete(IndexName(aCName, aIIndex));
}

// ---------------------------------------------------------
// FindNext stuff.
// ---------------------------------------------------------

inline void NVThing::ResetFindNext() const
{
	mIter = mMValues.end();
}

inline bool NVThing::FindNext(const char *aCName, const char* &aCValue, bool aBNotFoundException) const
{
	// Support repeated calls to FindNext to get subsequent items.
	// So you can have multiple items of the same name.
	if (mIter != mMValues.end() && mIter->first == aCName)
	{
		++mIter;
		if (mIter != mMValues.end() && mIter->first != aCName)
			mIter = mMValues.end();
	}
	else
	{
		mIter = mMValues.find(aCName);
	}

	if (mIter != mMValues.end() && mIter->second->mS)
	{
		aCValue = mIter->second->mS->c_str();
		return true;
	}
	else if (aBNotFoundException)
	{
		throw Common::MessageException(aCName) << " was not found";
	}
	else
		return false;		// not found or not a string.
}

bool NVThing::FindNext(const char *aCName, string &aSValue, bool aBNotFoundException) const
{
	const char *lC;
	if (FindNext(aCName, lC, aBNotFoundException))
	{
		aSValue = lC;
		return true;
	}
	else
		return false;
}

inline bool NVThing::FindNext(const char *aCName, NVThing *&aNvt, bool aBNotFoundException) const
{
	// Support repeated calls to Find to get subsequent items.
	// So you can have multiple items of the same name.
	if (mSLastFind == aCName)
	{
		++mIter;
		if (mIter != mMValues.end() && mIter->first != aCName)
			mIter = mMValues.end();
	}
	else
	{
		mIter = mMValues.find(aCName);
		mSLastFind = aCName;
	}

	if (mIter != mMValues.end() && mIter->second->mNvt)
	{
		aNvt = mIter->second->mNvt;
		return true;
	}
	else if (aBNotFoundException)
	{
		throw Common::MessageException(aCName) << " was not found";
	}
	else
		return false;		// not found or not a string.
}

inline bool NVThing::FindNext(const char *aCName, NVArchivable &aNva, bool aBNotFoundException) const
{
	NVThing *lNvt;
	if (FindNext(aCName, lNvt, aBNotFoundException))
	{
		aNva.FromNvt(*lNvt);
		return true;
	}
	else
		return false;
}

inline bool NVThing::FindNext(const char *aCName, int &aI, bool aBNotFoundException) const
{
	const char *lC;
	if (FindNext(aCName, lC, aBNotFoundException))
	{
		aI = atoi(lC);
		return true;
	}
	else
		return false;
}

inline bool NVThing::FindNext(const char *aCName, short &aS, bool aBNotFoundException) const
{
	int lI;
	if (FindNext(aCName, lI, aBNotFoundException))
	{
		aS = (short)lI;
		return true;
	}
	else
		return false;
}
inline bool NVThing::FindNext(const char *aCName, unsigned int &aUi, bool aBNotFoundException) const
{
	const char *lC;
	if (FindNext(aCName, lC, aBNotFoundException))
	{
		aUi = atoi(lC);
		return true;
	}
	else
		return false;
}

inline bool NVThing::FindNext(const char *aCName, char &aC, bool aBNotFoundException) const
{
	const char *lC;
	if (FindNext(aCName, lC, aBNotFoundException))
	{
		aC = *lC;
		return true;
	}
	else
		return false;
}

bool NVThing::FindNext(const char *aCName, double &aD, bool aBNotFoundException) const
{
	const char *lC;
	if (FindNext(aCName, lC, aBNotFoundException))
	{
		aD = atof(lC);
		return true;
	}
	else
		return false;
}

bool NVThing::FindNext(const char *aCName, float &aF, bool aBNotFoundException) const
{
	const char *lC;
	if (FindNext(aCName, lC, aBNotFoundException))
	{
		aF = static_cast<float>(atof(lC));
		return true;
	}
	else
		return false;
}

inline bool NVThing::FindNext(const char *aCName, int64_t &aI64, bool aBNotFoundException) const
{
	const char *lC;
	if (FindNext(aCName, lC, aBNotFoundException))
	{
		aI64 = atoll(lC);
		return true;
	}
	else
		return false;
}

inline bool NVThing::FindNext(const char *aCName, bool &aB, bool aBNotFoundException) const
{
	const char *lC;
	if (FindNext(aCName, lC, aBNotFoundException))
	{
		if (strncmp(lC, "true", 4) == 0)
			aB = true;
		else
			aB = false;

		return true;
	}
	else
		return false;
}

// Rules for NVThing files.
// NVThing files consist of alternating name and value lines.
// Indentions with the tab character indicate 'subobjects', embedded NVThings.

/*

Name
Ben
Date
11/06/68
Possessions
	Name0
	Guitar
	Name1
	Computer
	Name2
	Rug
Location
Columbus

*/



#endif
